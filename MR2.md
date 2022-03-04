Hi folks!
We apologize for the late postmortem, but better late than never…
First, we would like to thank everyone for this amazing competition. We all learned a lot throughout this journey. Special shout-out to the discord folks with whom we had very interesting discussions, to Stone, to the kaggle team and to our best competitors: the Toad’s Brigade!

TLDR: We trained an RL agent using PPO to maximize a hand-crafted dense reward function. A KL-divergence penalty is added to the PPO loss in order to distill previous best models. The agent receives an observation of size 32x32x|features| and output an action map of size 32x32x|actions|. The agent is modelized using a simple ResNet with 17 blocks of 32 convolutions with kernel of size 3x3, which is about 300 000 parameters (only!). During data generation, we sample previous iterations as opponents using Prioritized-Fictitious Self-Play (PFSP). We implemented the RL algorithms from scratch, without using open-sourced RL libraries.

I) Observations

Our features space consists of 37 features (28 related to information on the map and 9 general information, repeated across the map). These features have been carefully chosen using a behavior cloning pipeline as it was faster to iterate and try new things. Key learnings:
Feature normalisation and feature clipping are really important. We observed a real gap in performance when we resized all our features between roughly the same range ([0, 1] or [-1, 1]). Feature clipping ensures that the values the network will encounter on the leaderboard does not differ much from training, which helps for robustness. For instance, as carts were (sadly) not used, we decided to consider all units as workers and clip features accordingly.
When analyzing predictions, we discovered that workers wanted to build city tiles even when their cargo were not full but almost full (with 99 woods for exemple). At that time, we only had the sum of all ressource in cargo as a feature. The network had a hard time making a difference between 99 and actually 100 (where you can build city tiles). To mitigate that, we added boolean features that were actually action masks like is_cargo_full, is_case_buildable, map's edge… This helps by roughly 2 pts on the accuracy of the behavior cloning task. We now describe our features.

General information:

turn, turn divided by 359, range [0,1]
turn in the day/night cycle, turn % 40 divided by 39, range [0,1]
day / night, True if it is daylight, bool
research point related for each players
research point, divided by 200, range [0,1]
can farm coal, research point >= 50, bool
can farm uranium, research point >= 200, bool
Maps information:

ressources (wood, coal, uranium), divided max wood, coal, uranium resp., range [0,1]
map edges, 1 at the edge of the map 0 instead, bool
case buildable (no resource, no citytile), bool
Units:

unit's owner, 1 if our unit, -1 if it's opponent unit, 0 otherwise, range[-1, 1]
unit's cooldown, divided max worker cooldown, range [0,1]
unit's cargo (wood, coal, uranium, sum_of_ressource), divided by 100, range [0,1]
total fuel carried, divided by max fuel for a worker (4000), range [0,1]
cargo is full, cargo >= 100, bool
numbers of units on the case, 1 + 0.1 for every other unit (1.2 if there is 3 units), range [0, ~2]
will survive the night, True if the unit have enough resource to survive the current cycle night, bool
Citytiles:

city tile's owner, 1 if our city tiles, -1 if it's opponent city tiles, 0 otherwise, range[-1, 1]
city tile's cooldown, divided max city tile cooldown, range [0,1]
will survive the night, True if the city have enough fuel to survive the current cycle night, bool
will survive the game, True if the city have enough fuel to survive the game, bool
number of night steps that it can survive, min(fuel // city_light_upkeep, 60.) / 60., range [0, 1]
number of citytile you can add with your excess of fuel, min(max(fuel - (city_light_upkeep * night_left_count_total), 0) // (18 * night_left_count_total), 15.) / 15., range [0, 1]
In order to help the value model, we also add the resource (wood, coal, uranium) and units from the first step, we also add the unit, the city tiles and the difference of woods from the previous timestep.

II) Actions

Our agent controls all units with only one action map. This makes the whole action space tremendously large: N_UNIT_ACTION ** (map_width * map_height). This number is way orders of magnitude bigger than the number of atoms in the universe for N_UNIT_ACTION of 6 in 32x32 maps! Therefore we first tried to simplify the game by ignoring the transfer mechanism and implemented a very basic heuristic for citytiles (build if you can, otherwise research) and actually achieved good results with it on the LB. So we started our journey with only 6 actions: Do nothing, Go North, Go South, Go West, Go East, Build Citytile.

In our quest of simplifying the problem and reducing the action space, we masked unvalid actions. For example, we mask “Build Citytile” if the unit cargo is not full. When masking an action, we set its log-probability to -infinity to ensure it is not sampled. This was critical for RL to work at all, but also brought its bunch of challenges, see the “RL algorithm” part.

When thing started to go well, we decided to improve the agent with 2 ways:

1) Adding Transfert action to the action space. Instead of 4 transfer actions for each direction (which would have blown up the action space), we only added only one action for transfer. When the transfer action is sampled, we hard-coded its behavior. First, we always transfer the maximum possible amount of resources. Then, we give priority to resources with more fuel (uranium, then coal, then wood). Finally, if there are multiple potential receivers, we had a priority to the unit on city tiles then to the unit with more space left on his cargo. When creating those rules, we bottleneck the theoretical final performance of the RL agent. But in practice, it drastically reduces the action space, which saves a lot of compute.

2) As we didn't want to increase the actions space more than that, we decided to train a separate model for citytiles. This model had only 3 actions: Do nothing, Research, build workers (we never activated the cart mechanism). We used behavior cloning using data from the top 5. This helped us to have a good enough city tiles policy without the cost of a larger actions space. Sadly, by lack of time and resources, we never tried to add the citytiles actions into the main model.

III) Reward function

Reward functions are often tricky to get right (there is a lot of literature on this subject).

The most natural and purest learning signal is the win/loss reward function which is a binary sparse reward of 0 on all steps, -1 for loss and 1 for win on the very last step. However, this kind of reward signal gives rise to the well-known problem of credit assignment in RL. Because the reward is 0 on most of the steps, it makes it difficult for the algorithm to figure out what were the good moves that led to the win, and what were the bad ones. This problem is especially present in Lux, as one action is in fact multiple simultaneous unit actions. And it is very plausible that some units did great moves while others did not. We tried this reward function at the very beginning, starting from totally random play and without any kl-divergence term to distill previous models. It worked, but gave us at most a 1200 elo agent after multiple training days.

To partially deal with this credit assignment problem, we decided to switch to a hand-crafted dense reward function. The drawback of doing so is that the prior knowledge we encode in this reward function is often flawed. As an example, one could want to say that farming more than the opponent is great. However, there may be scenarios where you may want to let the opponent farm more and over-extend its city, because it will later run out of fuel. And even if we suppose that our prior knowledge was totally right, the way we encode it into the reward function is also often imperfect (e.g. who knows how should the farming reward signal be weighted compared to the win/loss reward signal?).

Our dense reward function is the first time derivative of a score i.e. r(t) = score(s_{t+1}) - score(s_t). The score function has multiple components.

Farming score: it is the sum of all resources farmed by the units since the beginning of the game. This score is normalized by the total initial resource amount of the map in order to not overweight maps with lots of resources compared to others.
Control score: we attribute each case of the map to one of the two players, given our estimation of who can get to the case first, taking cooldowns into account. If the two players can get to a case on the same turn, the case is “shared”. To compute who can get to location and when, we run a modified version of the Voronoid algorithm, taking opponent citytiles and units as obstacles. This control score is flawed, but still gives a good estimation of who controls what. Given this segmentation, we compute how much resources are controlled by a player, weighted by their distance (the farther a controlled resource is, the less it contributes to the total controlled resources. This gives us a control score, which is also normalized by the initial total map resources.
Citytiles score: given the current fuel amount of cities, we can compute how much citytiles will survive the whole game if we stopped playing. we add this number to the score.
Win/loss score: we add +50 for winning and -50 for losing a game to the score.

The total score function is then the weighted sum of all score sub scores:

score(s_t) = a * farming_score(s_t) + b * control_score(s_t) + c * citytiles_score(s_t) + d * win_loss_score(s_t).

IV) Transition function

The transition function gives the distribution of the next state given the current state and the performed action: s_{t+1} = T(s_t, a_t). In practice, this new state is computed by the game engine from kaggle. However, the engine was super slow. We spent a significant amount of time trying to optimize it, but did not get any significant improvements. If we had to do it again, we would re-implement a vectorized version of the engine directly on the GPU. Even after our optimizations, we spent 80% of the training time in the env.step call.

We used the SubProcVecEnv from OpenAi to run env in multi-process https://github.com/openai/baselines/blob/master/baselines/common/vec_env/subproc_vec_env.py.
We also tried to use the python environment. This required quite a lot of changes in our code. Unfortunately, this was not as fast as we expected. We had to do further work like pre-computing millions of initial maps to avoid generating them at train time. We also optimized our masking mechanism and our observation computation. With all those improvements, we roughly gained a factor of 2 in the time spent in the .step method.

Our main computer was using a RTX2080Ti as GPU and a i9-9900k processor.
In the end the data collection part was around 300 steps per second (which is a bit more than 1M steps per hour).

V) Model

In order to find a good architecture, we used a behavior cloning task on the Citytiles. Based on the assumption that if a model that performs well in BC it would also be good for RL. We have no proof that this is correct but DeepMind did the same for StarCraft, therefore we behavior-cloned them :).

When creating an image to image architecture only with convolution layers, one has to be very careful about the field of view of each output. It is important that each output sees the whole input observation, otherwise it cannot take everything into account when deciding what to do. We went through a bunch of different architectures. Namely: ResNet, Unet, Res-Unet, Attention Unet…

The best performing model was a fully convolutional ResNet, very similar to the one used in AlphaGo, but with fewer convolutions and fewer layers. We also got rid of the BatchNormalization layers as they are not good for on-policy algorithms. The problem with ResNet is that unlike Unet architectures, the feature map is never down-scaled. Therefore each forward pass requires a huge amount of computation. We observed that deeper and larger networks constantly performed better, but they would then become too slow for our RL algorithm. So we had to find the best compromise between speed and performance.

We observed that Gelu activation function gives a little performance boost compared to ReLU.

VI) RL algorithm

We use Proximal Policy Optimization from John Schulman that we coded from scratch (no use of external RL libraries). PPO is quite easy to implement. Moreover, it is an on-policy RL algorithm, which often has better asymptotic performance than off-policy RL algorithms, at the cost of sample efficiency. Therefore we went for it. However, if we knew how slow the environment was to sample, we would have gone for an off-policy algorithm (or alternatively we should have re-coded the game engine on GPU..) because even after 4 full days of training, we never reached any plateau in the agent’s performance.

In PPO there are two networks. The policy network and the value network. Both take the current state as input. When the input has a high dimension like images, it has been shown that sharing some of the first layers between the policy and the value is beneficial (Cobe K. Phasic Policy Gradient paper) because it helps to create good features. However in our experiment, we have not been able to do so. Every time we tried, the gradients from the value loss completely destroyed the policy. We therefore have two separate networks with their own optimizers.

Learning everything from random play in a pure self-play setting is very challenging and compute intensive. Moreover, it has been observed that higher performance and faster
training can be achieved in deep RL by first training an agent, then subsequently training a new agent on the identical task whilst performing policy distillation from the first agent (Czarnecki et al., 2019; Furlanello et al., 2018; Schmitt et al., 2018). We therefore re-use models obtained in previous iterations when training new ones with a kl-divergence loss in addition to the vanilla PPO loss.

We use a slightly modified version of PPO in order to mask invalid actions. To do so we had to augment the CategoricalDistribution from Pytorch. This has to be done properly, otherwise one might get into trouble with the on-policyness of the algorithm (we need to record what was the forbidden actions during environment interactions in order to evaluate the log probabilities of the policy properly!). Moreover, when computing the log probabilities, we sum the log probabilities of each unit-action during a step. This gives us the log probability of the whole step-action. Another possibility would have been to only use the unit-action log probability. It seemed to also work but it converged slower. We think that it is because it over-weights too much the steps where there are a lot of units. Finally, when computing the step-action log probability, we mask the input to ignore all positions where n_available_action <= 1. Therefore the policy gradient will only be computed using the positions where a decision has actually been made, and was not completely forced.

In all our experiments we use the AdamW optimizer, which is a correct version of Adam optimizer when we want to add weight decay regularization. We have a linear learning rate annealing schedule for the RL training (3e-3 to 3e-5).

VII) Matchmaking
In all our experiments, opponents are also models, but used in a deterministic manner (not sampling the predicted Gaussians but only using the mean) because the models play better this way.

In order to sanity check our implementation, we first tried to beat a fixed opponent. Once the opponent becomes too weak, we copy-paste the current model weights in the opponent model, and run the experiment again. The drawback of this method is the lack of diversity in opponents, which leads to an “opponent overfitting” of the current model.

Then, we tried pure self-play, in which the current model always matches itself. However this kind of training often leads to instabilities as well as RSP behaviors.

Finally, our ultimate training protocol is based on Prioritized Fictitious Self-Play (PFSP). In Fictitious Self-Play, we not only train against the current model, but also all its previous checkpoints. It has been done in the Dota 2 paper from OpenAI. We additionally added prioritization when sampling the checkpoints so that we would sample more checkpoints against which we have a small learning rate. For those who are interested, the next step in matchmaking would be to do league training, with exploiters as in the StarCraft paper from DeepMind. However it would have required way too much compute for my little computer.

VIII) Data augmentation:

When training CT models in BC, we augment the data with the 8 possible symmetries. Moreover, we slightly randomize the resources on the map of +- 1%.

We would have loved to use this kind of data augmentation for the RL algorithm. However, data augmentation is not possible when doing on-policy RL, as the agent would have acted differently in the augmented observation. We would have loved to explore this idea though, but we did not have enough time as RL experiments take so long.

IX) Comparisons to Toad’s Brigade approach:

Both submissions have a lot in common. We can learn quite a lot by comparing them.

Main similarities:

Reinforcement learning: The two submissions use RL, which seemed to be the key here. They use IMPALA, which is an off-policy algorithm using some off-policy correction tricks. This should achieve less good asymptotic performance but this has a higher sample efficiency. Given the slowness of the environment, this was definitely a good choice.
ResNet: Both approaches use a ResNet. We do not have shared parameters between the value network and the policy network while TB do. Also, TB has a way bigger network than us.
Distillation of past models: Shared in both sub.
Forbidden action masking: Shared in both sub.
Data augmentation: as TB are doing off-policy RL, they can do data augmentation in the RL phase (do they?). This is not possible for on-policy RL. However, at test time, we predict the logits for the 8 augmented maps and average them.
Main differences:

TB feature map masking: We do not use this trick. However, I am not sure this architecture choice makes much difference.
Reward function: They first started with a dense reward function to heat things up, then switched to a sparse one (win/loss), which is a really good idea. In our case we went the other way round. We first experimented with the sparse reward function and got very poor results. Then we switched to a dense reward function and things started to work, but we stuck to it. We should have changed it in the last iterations as we think the distillation loss would have helped the training enough.
CT in the RL training: We do not have the same model for the units and the citytiles, because we thought it would have been too difficult at first. We planed to do it when the RL became good enough with the unit model but again, got caught by the deadline.
Finally, we believe that the biggest difference between the two approaches is scale. Both in terms of training time (4 days vs 20 days) and number of parameters in the network (300k vs XM).

Thanks again for this amazing competition, I hope you learned a bit reading this postmortem. Feel free to ask questions!

Best,
RLIAYN