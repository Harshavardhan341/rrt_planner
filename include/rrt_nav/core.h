#include <ros/ros.h>
#include <nav_msgs/GetMap.h>
#include <bits/stdc++.h>
#include <utility>
#include <iostream>
#include <list>
#include<visualization_msgs/Marker.h>
#define STEP_DISTANCE 4
#define RADIUS 3
#include <math.h>

using namespace std;
class RRT{
    private:
        
        ros::ServiceClient get_map_client;
        nav_msgs::OccupancyGrid map;
        geometry_msgs::Point start,goal;
        ros::Publisher marker_pub;
        
    public:    
        struct Node{
            //store stl pair of x,y
            geometry_msgs::Point point;
            //store parent node
            Node *parent;  
           
        };
        list <Node> rrt_;
        list <Node> path;
        public:
        RRT(ros::NodeHandle *);
        //~RRT();
        nav_msgs::OccupancyGrid get_map_data();//get occupancy grid data
        bool isValid(geometry_msgs::Point);//check if point is occupied
        geometry_msgs::Point generate_random_pt();
        Node nearest(geometry_msgs::Point,list <Node> &);//get nearest node
        //bool isValid(pair<int,int>);//check if node is valid
        bool close2goal(Node &);//check is point is close enough to goal
        Node new_conf(Node *,geometry_msgs::Point);
        void main_algo();//algorithm fn list<Node>
        void get_path(Node &);

        
};
