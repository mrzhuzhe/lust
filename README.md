# Lux game

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
2. https://www.kaggle.com/dwight9339/rule-based-q-learning-agent

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