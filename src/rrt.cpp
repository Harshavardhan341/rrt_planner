
#include <rrt_nav/core.h>
#define ITERATIONS 1000

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
    float x = p.x+10;
    float y =p.y+10;
    
    int nx = x/0.05;
    int ny = y/0.05;
    //std::cout<<nx<<" "<<ny<<std::endl;

    int index = (ny)*384 + nx ;

    //printf("%d",index);

    if(this->map.data[index]==0)
        return true;
    return false;

}

<<<<<<< HEAD
RRT::Node RRT::nearest(geometry_msgs::Point p, list <Node> &tree)
=======
RRT::Node RRT::nearest(geometry_msgs::Point p, list <Node> tree)
>>>>>>> main
{   
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

    return nearest;
}
bool RRT::close2goal(RRT::Node &n)
{   
    if(sqrt(pow(n.point.x-this->goal.x,2)+pow(n.point.y-this->goal.y,2))<0.1)
        return true;
    return false;
}
RRT::Node RRT::new_conf(Node *nearest,geometry_msgs::Point p)
{   
    Node new_conf;
    float theta = atan2((p.y-nearest->point.y),(p.x-nearest->point.x));
    new_conf.point.x = nearest->point.x+STEP_DISTANCE*cos(theta);//set coordinates of new node
    new_conf.point.y = nearest->point.y+STEP_DISTANCE*sin(theta);
<<<<<<< HEAD
    
    
    new_conf.parent = nearest;//set parent 
    
=======
    new_conf.parent = nearest;//set parent 
>>>>>>> main
    return new_conf;

}
geometry_msgs::Point RRT::generate_random_pt()
{
    geometry_msgs::Point p;
    p.x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(384*0.05)));
    p.y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(384*0.05)));
    return p;

}
void RRT::get_path(RRT::Node &n)
{ 
    
    
    
    do
    {
        this->path.push_back(n);
        n=*n.parent;

    }while(n.parent!=0);
    list <Node>::iterator it;
    for(it=this->path.begin();it!=this->path.end();++it)
    {
        cout<<"X: "<<it->point.x<<endl;
        cout<<"Y: "<<it->point.y<<endl;
    }   

}
void RRT::main_algo()
{   list<Node> tree;
    geometry_msgs::Point random;
<<<<<<< HEAD
    Node *start_node = new Node;
    start_node->point=start;
    start_node->parent=NULL;
    cout<<start_node<<endl;
    tree.push_back(*start_node);//add start node to tree
    this->map = get_map_data();
=======
    Node start_node;
    start_node.point=start;
    start_node.parent=NULL;
    
    tree.push_back(start_node);//add start node to tree
>>>>>>> main

    int i;

    for(i=0;i<=ITERATIONS;i++)
    {   
        random = generate_random_pt();
<<<<<<< HEAD

        
=======
>>>>>>> main

        
        this->map = get_map_data();


        if(isValid(random))
        {   
<<<<<<< HEAD
            //cout<<"Valid point selected"<<endl;
            Node* nearest_node = new Node;
            *nearest_node = nearest(random,tree);
            
            Node* new_node = new Node;
            *new_node = new_conf(nearest_node,random);



            cout<<"NEW NODE ADDRESS: "<<new_node<<endl;
            cout<<"NEW NODE PARENT x: "<<new_node->parent->point.x<<endl;
            cout<<"NEW NODE PARENT : "<<new_node->parent<<endl;

            
            tree.push_back(*new_node);
            if(close2goal(*new_node))
                {   cout<<"Goal Reached";
                    cout<<"X "<<new_node->point.x;
                    cout<<"Y "<<new_node->point.y;
                    this->get_path(*new_node);
                    break;
                }
=======
            Node nearest_node = nearest(random,tree);
            Node* new_node = new Node;

            float theta = atan2((random.y-nearest_node.point.y),(random.x-nearest_node.point.x));
            new_node->point.x = nearest_node.point.x+STEP_DISTANCE*cos(theta);//set coordinates of new node
            new_node->point.y = nearest_node.point.y+STEP_DISTANCE*sin(theta);
            new_node->parent = &nearest_node;
            
            cout<<"Parent: "<<new_node->parent<<endl;
            tree.push_back(*new_node);
            
>>>>>>> main
        }
        else
        continue;

    }   
    
}
int main(int argc, char** argv)
{
    ros::init(argc,argv,"rrt_star_node");
    ros::NodeHandle nh;
    RRT r(&nh);

    
    r.main_algo();


      
}
