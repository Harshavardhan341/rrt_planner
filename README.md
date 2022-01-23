# RRT PLANNER
This repository contains an RRT and an RRT star based global planner which uses a 2D occupancy grid for the purpuse of checking validity of states or points. 
More explaination regarding the algorithm can be found [here](https://en.wikipedia.org/wiki/Rapidly-exploring_random_tree).
I am working on the visualization for this on RViz and that will be up shortly.

## Running the code
1. Building the code
```
cd ~/catkin_ws/src
git clone https://github.com/Harshavardhan341/rrt_planner 
cd ..
catkin_make 
```
2. Running the code 
```
cd catkin_ws
source ./devel/setup.bash
roslaunch rrt_nav planner.launch

```
 
