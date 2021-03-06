
#include <rrt_nav/core.h>
#define ITERATIONS 20000

using namespace std;

RRT::RRT(ros::NodeHandle *nh)
{   get_map_client = nh->serviceClient<nav_msgs::GetMap>("/static_map");
    marker_pub=nh->advertise<visualization_msgs::Marker>("vis_marker",10);
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

RRT::Node RRT::nearest(geometry_msgs::Point p, list <Node> &tree)
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
    if(sqrt(pow(n.point.x-this->goal.x,2)+pow(n.point.y-this->goal.y,2))<0.5)
        return true;
    return false;
}
RRT::Node RRT::new_conf(Node *nearest,geometry_msgs::Point p)
{   
    Node new_conf;
    float theta = atan2((p.y-nearest->point.y),(p.x-nearest->point.x));
    new_conf.point.x = nearest->point.x+STEP_DISTANCE*cos(theta);//set coordinates of new node
    new_conf.point.y = nearest->point.y+STEP_DISTANCE*sin(theta);
       
    new_conf.parent = nearest;//set parent 
    
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
    //for(it=this->path.begin();it!=this->path.end();++it);

    
    cout<<"Goal X "<<path.begin()->point.x;
    cout<<"Goal Y"<<path.begin()->point.y<<endl;

    cout<<"Start X "<<path.end()->point.x;
    cout<<"Start Y"<<path.end()->point.y;
    
}
void RRT::main_algo()
{   list<Node> tree;
    geometry_msgs::Point random;
    Node *start_node = new Node;
    start_node->point=start;
    start_node->parent=NULL;

    tree.push_back(*start_node);//add start node to tree
   
    this->map = get_map_data();
 

    int i;

    for(i=0;i<=ITERATIONS;i++)
    {   
        random = generate_random_pt();

        if(isValid(random))
        {   
        
        //this->map = get_map_data();


           

            Node* nearest_node = new Node;

            *nearest_node = nearest(random,tree);


            Node* new_node = new Node;
            *new_node = new_conf(nearest_node,random);

            
            //cout<<"NEW NODE X: "<<new_node->point.x<<endl;
            //cout<<"NEW NODE PARENT x: "<<new_node->parent->point.x<<endl;
            //cout<<"NEW NODE PARENT : "<<new_node->parent<<endl;

            
            tree.push_back(*new_node);
            //cout<<"pushed back"<<endl;

            if(close2goal(*new_node))
                {   cout<<"Goal Reached";
                    //cout<<"X "<<new_node->point.x;
                    //cout<<"Y "<<new_node->point.y;
                    this->get_path(*new_node);
                    break;
                }
        }
        else continue;
      

    }   
    
}
int main(int argc, char** argv)
{
    ros::init(argc,argv,"rrt_star_node");
    ros::NodeHandle nh;
    RRT r(&nh);

    
    r.main_algo();


      
}