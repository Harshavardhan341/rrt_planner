cmake_minimum_required(VERSION 3.0.2)
project(rrt_nav)

find_package(catkin REQUIRED COMPONENTS
  nav_msgs
  roscpp
  std_msgs
  sensor_msgs
)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS nav_msgs roscpp std_msgs
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(rrt_planner_node src/rrt.cpp)
target_link_libraries(rrt_planner_node ${catkin_LIBRARIES})