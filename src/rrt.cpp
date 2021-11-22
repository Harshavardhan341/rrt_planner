

#include <rrt_nav/core.h>
#define ITERATIONS 100

using namespace std;

RRT::RRT(ros::NodeHandle *nh)
{   get_map_client = nh->serviceClient<nav_msgs::GetMap>("/static_map");
    start.x = -6;
    start.y = 3;
    goal.x = 7;
    goal.y=7;
}
nav_msgs::OccupancyGrid RRT::get_map_data()
{   nav_msgs::GetMap srv;            
    if(get_map_client.call(srv))
        return srv.response.map;        

}
bool RRT::isValid(geometry_msgs::Point p)
{
    p.x+=10;
    p.y+=10;
    map=get_map_data();
    int nx = p.x/0.05;
    int ny = p.y/0.05;
    //std::cout<<nx<<" "<<ny<<std::endl;

    int index = (ny)*384 + nx ;

    //printf("%d",index);

    if(map.data[index]==0)
        return true;
    return false;

}

RRT::Node RRT::nearest(geometry_msgs::Point p, list <Node> tree)
{   cout<<"enters nearest"<<endl;
    double dist=INFINITY;
    Node nearest;
    list <Node>::iterator it;
    for(it=tree.begin();it!=tree.end();++it)
    {
        if(sqrt(pow(p.x-it->point.x,2)+pow(p.y-it->point.y,2))<dist)
        {
            dist=sqrt(pow(p.x-it->point.x,2)+pow(p.y-it->point.y,2));
            nearest = *it;
        }
        else
            continue;
    }
    cout<<"nearest x"<<nearest.point.x<<endl;
    cout<<"nearest y"<<nearest.point.y<<endl;
    return nearest;
}
bool RRT::close2goal(geometry_msgs::Point p)
{
    if(sqrt(pow(p.x-this->goal.x,2)+pow(p.y-this->goal.y,2))<0.1)
        return true;
    return false;
}
RRT::Node RRT::new_conf(Node nearest,geometry_msgs::Point p)
{   cout<<"enters new_conf"<<endl;
    Node new_conf;
    float theta = atan2((p.y-nearest.point.y),(p.x-nearest.point.x));
    new_conf.point.x = nearest.point.x+STEP_DISTANCE*cos(theta);//set coordinates of new node
    new_conf.point.y = nearest.point.y+STEP_DISTANCE*sin(theta);
    new_conf.parent = &nearest;//set parent 
    return new_conf;

}
geometry_msgs::Point RRT::generate_random_pt()
{
    geometry_msgs::Point p;
    p.x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(384*0.05)));
    p.y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(384*0.05)));
    return p;

}
list<RRT::Node> RRT::main_algo()
{   list<Node> tree;
    geometry_msgs::Point random;
    Node start_node,nearest_node,new_node;
    start_node.point=start;
    start_node.parent=NULL;
    cout<<start_node.parent<<endl;
    tree.push_back(start_node);//add start node to tree



    for(int i=0;i<=ITERATIONS;i++)
    {   
        random = generate_random_pt();
        cout<<"random.x "<<random.x<<endl;
        cout<<"random.y "<<random.y<<endl;
        



        if(isValid(random))
        {   cout<<"Valid point selected"<<endl;
            nearest_node = nearest(random,tree);
            new_node = new_conf(nearest_node,random);
            tree.push_back(new_node);
        }
        else
        continue;

    }   
    return tree;
}
int main(int argc, char** argv)
{
    ros::init(argc,argv,"rrt_star_node");
    ros::NodeHandle nh;
    RRT r(&nh);
    
    r.main_algo();

      
}
