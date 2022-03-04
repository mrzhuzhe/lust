# Lux game

## 2022/02/22 

### checklist
1. env
    env wrapper

    reward:
        according to readme seems only use StatefulMultiReward/PunishingExponentialReward
    action:
        how action mask work ?
    paddingwrapper
        how mask work ?
2. observe
    FixedShapeContinuousObs what different between v1 and v2 ?
    init on obs observed on wrapper
    how to padding ?

3. loss 
    entropy_loss + upgo_pg_losss + teacher_kl_loss + vtrace_pg_loss
    https://app.diagrams.net/#G1irvJtpYfybkOmW_SkJivFvJGYIG1bHdv
4. trian 
    model 
    why not norm

5. log 
6. eval 
7. test

### question:

1. v-trace can be used in q-learning ?
2. upgo with no paper ?
3. why cannot use batchnorm
4. what is teacher student
5. how to reward shaping
6. how to find improvement
7. how to self play




### version
1. version1 down layer 8 to 4 hiddenunit 128 to 64




















## 比赛完后记得删掉 
npm install -g @lux-ai/2021-challenge@latest

## 可视化
https://2021vis.lux-ai.org/

## API 
https://github.com/Lux-AI-Challenge/Lux-Design-2021/tree/master/kits

## cli
sudo lux-ai-2021 main.py main-opposite.py --out=./replays/eplay.json

## 历史方案有两个ml base的 model
1. 5th https://www.kaggle.com/c/halite/discussion/183704
2. 8th https://www.kaggle.com/c/halite/discussion/183312  https://www.youtube.com/watch?v=WjFdD7PDGw0&t=3843s
本次比赛也有imitate 和 rl的model
1. Imitate https://www.kaggle.com/shoheiazuma/lux-ai-with-imitation-learning
2. q-learning https://www.kaggle.com/dwight9339/rule-based-q-learning-agent
3. PPO https://www.kaggle.com/c/lux-ai-2021/discussion/267351  notebook https://www.kaggle.com/glmcdona/reinforcement-learning-openai-ppo-with-python-game?scriptVersionId=76133670

## 方向

1. 对不同地图做处理
2. 对对手形式做处理

- 地图
1. 如果铀和森林里离的太远
2. 如果敌人离得太近

- 对采集到的不同资源，白天黑夜做处理 
1. 如果有木头 倾向于远距离行走 和造房子 
2. 如果有煤倾向于运回城市  
3. 如果有铀，倾向于运回城市

- 对目标不同资源
1. 木头倾向于节能
2. 油倾向于保护
3. 铀倾向于保护

- 针对对手形式
1. 护林模式
2. 远距离扩张

- 分阶段
1. 前期重争夺资源和研究
2. 后期重保护城市

## Imitate learning

1. 对于模仿学习和IRL和ppo和behaveclone的区别，模仿学习不同于behave clone的区别在于考虑了多层state和action的关系，和irl的区别在于增加了正向rl让表现至少好于irl，ppo应该只是再正向中增加了采样方式
2. actor critic 只是一种demostator 的 gan解释
3. PPO 的reward 是人工制定的？？？？
4. 试一下SB3 https://github.com/araffin/rl-tutorial-jnrr19/tree/sb3