#include<SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#include<deque>
#include<time.h>
#include<unordered_map>
#include<vector>
#include<limits>
#include<algorithm>
#include<string>
#include<math.h>
#include<stdlib.h>

using namespace std;

// Constants
#define PI 3.14
#define ROT_THR 1
#define CRUMB_SIZE 5
#define MAX_SPEED 2
#define NUM_BOUND_WALLS 4
#define NUM_ROOM_WALLS 7


class SteeringBehaviour
{
    public:
        void virtual calculate_velocity(sf::Vector2f curr, sf::Vector2f target) = 0;
        sf::Vector2f virtual get_position() = 0;
        void virtual set_position1(sf::Vector2i position) = 0;

};

class Position : SteeringBehaviour
{
        public:
        float radiusDeceleration = 100.0, radiusSatisfaction = 1.0, timeToTargetVelocity = 10.0;
        sf::Vector2f target, initial;
        sf::Vector2f position;

        void set_position1(sf::Vector2i position)
        {
            this->position.x = position.x;
            this->position.y = position.y;
        }

        sf::Vector2f get_position()
        {
            return this->position;
        }
};

class Orientation : SteeringBehaviour
{
    Orientation()
    {
        cout<<"I am in orientation";
    }
};

class Velocity : SteeringBehaviour
{
    public:
        sf::Vector2f velocity, goalVelocity;
        float maxAcceleration = 1;

        
        sf::Vector2f normalize(sf::Vector2f vec)
        {
            float dist = sqrt( pow(vec.x, 2.0) + pow(vec.y, 2.0));

            vec.x /= dist;
            vec.y /= dist;
            return vec;
        }

        void calculate_velocity(sf::Vector2f curr, sf::Vector2f target)
        {
            cout<<"I am in calculate velocity";
        }


};

class Rotation: SteeringBehaviour
{
    public:
        float rotation, maxRotation = 0.3, reqRotation, rotationSatisfaction = 0.0001, rotationDeceleration = 0.20;
        float targetRotation, initialRotation = 0;
        float timeToTargetRotation = 1.5;
};

class Sprite : Velocity,  Position, Rotation
{

    private:
        sf::Texture texture;
    public:
        bool change_room = false;
        sf::CircleShape obj;
        bool vel_flag = false;

        // Flag to check whether sprite is exhibiting decision tree behaviour or normal clicking behaviour
        bool dec_click;

    Sprite(bool isMonster)
    {
        this->obj = obj;
        this->position = obj.getPosition();
        this->velocity = sf::Vector2f(0.0f, 0.0f);

        if(isMonster)
        {
            this->obj.setRadius(10.0f);
            this->obj.setFillColor(sf::Color(255, 0, 0));
            this->obj.setPosition(360,300);
        }
        else
        {
            
            this->obj.setRadius(7.0f);
            this->obj.setFillColor(sf::Color(0, 0, 0));
            this->obj.setPosition(50,50);
        }
    }

    void reset_velocity()
    {
        this->velocity = sf::Vector2f(0.0,0.0);
    }

    void set_position(sf::Vector2f position)
    {
        this->obj.setPosition(position);
        // this->obj.position.x = position.x;
        // this->position.y = position.y;
    }

    void set_position1(sf::Vector2i position)
    {
        this->position.x = position.x;
        this->position.y = position.y;
    }

    sf::Vector2f get_position()
    {
        return obj.getPosition();
    }
    
    sf::Vector2f get_velocity()
    {
        return this->velocity;
    }

    void set_vel_flag()
    {
        vel_flag = !vel_flag;
    }

    void set_vel_flag(bool flag)
    {
        vel_flag = flag;
    }

    bool get_vel_flag()
    {
        return vel_flag;
    }

    float get_rotation()
    {
        return this->rotation;
    }

    void set_rotation(float rotation)
    {
        this->rotation = rotation;
    }

    sf::Vector2f getTarget()
    {
        return this->target;
    }

    void set_target(sf::Vector2f target)
    {
        this->target = target;
    }


    void set_initial()
    {
        this->initial = get_position();
    }

    sf::Vector2f getInitial()
    {
        return this->initial;
    }

    float get_initial_rotation()
    {
        return this->initialRotation;
    }

    void set_initial_rotation(float rotation)
    {
        this->initialRotation = rotation;
    }

    float get_target_rotation()
    {
        return this->targetRotation;
    }

    void set_target_rotation(float rotation)
    {
        this->targetRotation = rotation;
    }

    bool get_change_room()
    {
        return change_room;
    }

    void set_change_room(bool change_room)
    {
        this->change_room = change_room;
    }

    void move()
    {
        calculate_velocity(this->initial, this->target);
        // if (isnan(this->initial.x)||isnan(this->initial.y)){
        //     cout<<"Initial is nan"<<endl;
        // }
        // if (isnan(this->target.x)||isnan(this->target.y)){
        //     cout<<"Target is nan"<<endl;
        // }
        if(isnan(this->velocity.x)||isnan(this->velocity.y)){
            // cout<<"Velocity is nan"<<endl;
            cout<<"initial: "<<this->initial.x<<" "<<this->initial.y<<endl;
            cout<<"target: "<<this->target.x<<" "<<this->target.y<<endl;
            exit(0);
        }
        if(abs(targetRotation - this->rotation) > radiusSatisfaction)
        {
            calculate_rotation();
            obj.setRotation(this->rotation);
            this->initialRotation = this->rotation;
        }
        
        sf::Vector2f location = get_position() + this->velocity;
        obj.setPosition(location);
        this->initial += this->velocity;
        // if (isnan(this->initial.x)||isnan(this->initial.y)){
        //     cout<<"Velocity is nan"<<endl;
        // }
    }

    void calculate_velocity(sf::Vector2f curr, sf::Vector2f final)
    {
        float goalSpeed, goalSpeed1;
        
        sf::Vector2f new_distance = final - curr;
        if(isnan(new_distance.x)||isnan(new_distance.y)){
            cout<<"New distance is nan"<<endl;
            exit(0);
        }
        float req_speed = sqrt( pow(new_distance.x, 2.0) + pow(new_distance.y, 2.0) );
        if(isnan(req_speed)){
            cout<<"Req speed is nan"<<endl;
            exit(0);
        }

        if(req_speed < radiusSatisfaction) {if (vel_flag) set_vel_flag(); goalSpeed = 0; change_room = true;}
        else if(req_speed > radiusDeceleration) goalSpeed = MAX_SPEED;
        else goalSpeed = MAX_SPEED * req_speed / radiusDeceleration;

        if(isnan(goalSpeed)){
            cout<<"Goal speed is nan"<<endl;
            exit(0);
        }
        goalVelocity = new_distance;
        if(isnan(goalVelocity.x)||isnan(goalVelocity.y)){
            cout<<"Goal Velocity is nan"<<endl;
            exit(0);
        }
        
        goalVelocity = normalize(goalVelocity);
        if(isnan(goalVelocity.x)||isnan(goalVelocity.y)){
            cout<<"Normalized Goal Velocity is nan"<<endl;
            exit(0);
        }

        // if(isnan(goalVelocity.x)||isnan(goalVelocity.y)){
        //     cout<<"Goal velocity is nan"<<endl;
        // }

        //Handling movement when mouse is clicked mid-sprite movement
        if (vel_flag)
        {
            
            sf::Vector2f newGoalVelocity = sf::Vector2f(0.0, 0.0);

            if (this->rotation < 0)
            {
                newGoalVelocity.x = - this->velocity.x * cos ( this->rotation - this->initialRotation ) + this->velocity.y * sin ( this->rotation - this->initialRotation );
                newGoalVelocity.y = - this->velocity.x * sin ( this->rotation - this->initialRotation ) - this->velocity.y * cos ( this->rotation - this->initialRotation );
            }
            else
            {
                newGoalVelocity.x = this->velocity.x * cos ( this->rotation - this->initialRotation ) - this->velocity.y * sin ( this->rotation - this->initialRotation );
                newGoalVelocity.y = this->velocity.x * sin ( this->rotation - this->initialRotation ) + this->velocity.y * cos ( this->rotation - this->initialRotation);
            }
            if(isnan(newGoalVelocity.x)||isnan(newGoalVelocity.y)){
                cout<<this->rotation<<endl;
                cout<<"New goal velocity before normalization is nan"<<endl;
                exit(0);
            }
            // cout<<"New Goal Velocity: "<<newGoalVelocity.x<<" "<<newGoalVelocity.y<<endl;
            if(newGoalVelocity.x>0 && newGoalVelocity.y>0)
            {
                newGoalVelocity = normalize(newGoalVelocity);
            }
            if(isnan(newGoalVelocity.x)||isnan(newGoalVelocity.y)){
                cout<<this->rotation<<endl;
                cout<<"New goal velocity is nan"<<endl;
                exit(0);
            }
            
            newGoalVelocity *= goalSpeed;
            goalVelocity *= goalSpeed;
            this->velocity = (goalVelocity - newGoalVelocity) / timeToTargetVelocity;
            if(isnan(this->velocity.x)||isnan(this->velocity.y)){
                cout<<"Velocity is nan"<<endl;
                exit(0);
            } 
            this->target -= this->velocity;
            if(isnan(this->target.x)||isnan(this->target.y)){
                cout<<"Target - velocity is nan"<<endl;
                exit(0);
            }
            set_target_rotation(curr,this->target);
        }
        else
        {
            goalVelocity *= goalSpeed;
            this->velocity = (goalVelocity - this->velocity) / timeToTargetVelocity; 
            if(isnan(this->velocity.x)||isnan(this->velocity.y)){
                cout<<"Velocity is nan"<<endl;
                exit(0);
            } 
        }
        
        this->target = final;
    }

    int map_to_range(int rotation)
    {
        int r = rotation % 360;
        if(abs(r) <= 180) return r;
        else if (r > 180) return 180 - r;
        else return r + 180;
    }

    void set_target_rotation(sf::Vector2f curr, sf::Vector2f final)
    {
        sf::Vector2f new_distance = final - curr;

        reqRotation = float(atan2 (new_distance.y, new_distance.x) * 180.0 / PI);

        int rr = reqRotation;
        float decimal = reqRotation - rr;
        reqRotation = map_to_range(rr) + decimal;
        this->targetRotation = reqRotation;

    }

    float mod_360(float value)
    {
        int mul = 1;
        if(value < 0) mul = -1;

        int rv = mul*value;

        float decimal = value -  rv;
        
        float v =  ((rv % 360) + decimal) * mul;

        return v;
    }

    void calculate_rotation()
    {
        float goalRotation;
        
        float reqRotation1 = targetRotation - this->rotation;

        if (min(abs(reqRotation1),abs(360+reqRotation1)) == abs(reqRotation1)) reqRotation=reqRotation1;
        else reqRotation = mod_360(360+reqRotation1);
        

        if( abs(reqRotation) < rotationSatisfaction) goalRotation = 0;
        else if(abs(reqRotation) > rotationDeceleration) { if(reqRotation > 0) goalRotation = maxRotation; else goalRotation = -maxRotation; }
        else goalRotation = maxRotation * reqRotation / rotationDeceleration;

        goalRotation /= timeToTargetRotation;

        if (goalRotation < 0) goalRotation = max(goalRotation, -maxRotation);
        else goalRotation = min(goalRotation, maxRotation);

        // Converting angles to interger to mod by 
        int rr = goalRotation;
        float decimal = goalRotation - rr;
        int int_rot = this->rotation;
        float rot_decimal = this->rotation - int_rot;
        this->rotation =  (int_rot + rr) % 360 + decimal + rot_decimal;
    }
};

class crumb : sf::CircleShape
{
    public:

        crumb(bool isMonster)
        {
            this->setRadius(5.0f);
            srand(time(NULL));
            if(!isMonster) this->setFillColor(sf::Color(0,0,255));
            else this->setFillColor(sf::Color(0,250,150));
            this->setPosition(-100, -100);
        }

        //tell breadcrumb to render self, using current render window
        void draw(sf::RenderWindow* window)
        {
            window->draw(*this);
        }

        //set position of breadcrumb
        void drop(float x, float y)
        {
            this->setPosition(x, y);
        }

        //set position of breadcrumb
        void drop(sf::Vector2f position)
        {
            this->setPosition(position);
        }

        sf::Vector2f get()
        {
            return this->getPosition();
        }

};

// Attributes for room class are defined here
class Room
{
    public:
        int room_id;

        sf::Vector2f top_left, top_right, bottom_right, bottom_left;
        sf::Vector2f room_centre;

        vector <sf::Vector2f> room_entry;
        int num_entries;

        sf::Vector2f wander_point;
        vector <int> adjacent_rooms;

        Room(vector <sf::Vector2f> coordinates, int room_id, vector <sf::Vector2f> room_entry, vector <int> adjacent_rooms)
        {
            this->top_left = coordinates[0];
            this->top_right = coordinates[1];
            this->bottom_right = coordinates[2];
            this->bottom_left = coordinates[3];

            // Calculating coordinate of room centre
            room_centre.x = (this->top_left.x + this->top_right.x) / 2;
            room_centre.y = (this->top_left.y + this->bottom_left.y) / 2;

            this->room_id = room_id;
            for(auto re: room_entry)
                this->room_entry.push_back(re);

            for(auto room: adjacent_rooms)
                this->adjacent_rooms.push_back(room);

            this->num_entries = room_entry.size();
        }


        sf::Vector2f get_wander_point()
        {
            // cout<<"Room ID: "<<room_id<<endl;
            sf::Vector2f new_wander_point;

            new_wander_point.x = (rand() % (int)(this->bottom_right.x - this->bottom_left.x - 20)) + bottom_left.x + 10;
            new_wander_point.y =  (rand() % (int)(this->bottom_left.y - this->top_left.y - 20)) + top_left.y + 10;

            while(new_wander_point.x == wander_point.y || new_wander_point.y == wander_point.y)
            {
                new_wander_point.x = (rand() % (int)(this->bottom_right.x - this->bottom_left.x - 20)) + bottom_left.x + 10;
                new_wander_point.y =  (rand() % (int)(this->bottom_left.y - this->top_left.y - 20)) + top_left.y + 10;
            }
            wander_point.x = new_wander_point.x;
            wander_point.y = new_wander_point.y;
            
            // cout<<"TL Y:"<<this->top_left.y<<"BL Y:"<<bottom_left.y<<"BR X:"<<bottom_right.x<<"BL X:"<<bottom_left.x<<endl;

            // cout<<wander_point.y<<" "<<wander_point.x<<endl;
            return wander_point;
        }

};

class Graph
{
    public:

        // Graph data structure, unordered map of unordered maps

        unordered_map<int, const unordered_map<int, int> > vertices;

        void add_vertex(char vertex, const unordered_map<int, int>& edges)
        {
            vertices.insert( unordered_map <int, const unordered_map<int, int>>::value_type(vertex, edges));
        }

        vector<int> A_star(int first, int final)
        {
            unordered_map<int, int> previous;
            unordered_map<int, int> weights;
            
            vector<int> nodes;
            vector<int> path;
            
            auto comparator = [&] (int left, int right) { return weights[left] > weights[right]; };

            for (auto& vertex : vertices)
            {
                if (vertex.first == first) weights[vertex.first] = 0;
                else weights[vertex.first] = numeric_limits<int>::max();
                
                nodes.push_back(vertex.first);  
                push_heap(begin(nodes), end(nodes), comparator);
            }

            
            while (!nodes.empty())
            {
                pop_heap(begin(nodes), end(nodes), comparator);

                char closest = nodes.back();
                nodes.pop_back();
                
                if (closest == final)
                {
                    while (previous.find(closest) != end(previous))
                    {
                        path.push_back(closest);
                        closest = previous[closest];
                    }
                    
                    break;
                }
                
                if (weights[closest] == numeric_limits<int>::max()) break;
                
                for (auto& neighbor : vertices[closest])
                {
                    int alt = weights[closest] + neighbor.second;
                    if (alt < weights[neighbor.first])
                    {
                        weights[neighbor.first] = alt;
                        previous[neighbor.first] = closest;
                        make_heap(begin(nodes), end(nodes), comparator);
                    }
                }
            }
              
            return path;
        }
};

// Everything in the viewport is a room class, seperated by a wall class
class Wall : sf::RectangleShape
{   
    public:

    Wall(sf::Vector2f position, sf::Vector2f size, float rotation)
    {
        this->setFillColor(sf::Color::Green);
        this->setPosition(position);
        this->setSize(size);
        this->setRotation(rotation);
    }

    void draw(sf::RenderWindow* window)
    {
        window->draw(*this);
    }

    float get_rot()
    {
        return this->getRotation();
    }

    private:

    void setPos(sf::Vector2f position)
    {
        this->setPosition(position);
    }

    sf::Vector2f get_pos()
    {
        return this->getPosition();
    }


};

struct DecisionTree
{
    int node_id;
    
    DecisionTree *left;
    DecisionTree *right;
    DecisionTree() : node_id(0), left(nullptr), right(nullptr) {}
    DecisionTree(int x) : node_id(x), left(nullptr), right(nullptr) {}
    DecisionTree(int x, DecisionTree *left, DecisionTree *right) : node_id(x), left(left), right(right) {}
};

struct BehaviourTree
{
    int node_id, order_id;
    
    vector <struct BehaviourTree*> child;
    int prev_node;
    bool prev_output, prev_node_finish;

    struct BehaviourTree* parent;

    BehaviourTree(int node_id, int order_id)
    {
        this->order_id = order_id;
        this->node_id = node_id;
        this->prev_output = false;
        this->prev_node = -1;
        this->prev_node_finish = false;
    }

    void refresh()
    {
        this->prev_output = false;
        this->prev_node = -1;
        this->prev_node_finish = false;
    }
};

int main()
{
    // Creating the window and intial variables
    sf::RenderWindow window(sf::VideoMode(640, 480), "HW4 Part 2");
    Sprite sprite(false), monster(true);
    srand(time(0));
    sf::Clock clock, monster_clock;

    float start_time = clock.getElapsedTime().asSeconds();
    float monster_start_time = monster_clock.getElapsedTime().asSeconds();

    // Breadcrumbs

    deque <crumb> breadcrumbs, monster_breadcrumbs;
    sf::Vector2f latest_crumb_pos = sf::Vector2f (0.f, 0.f), monster_latest_crumb_pos = sf::Vector2f(0,0);
    int crumb_count = 0, monster_crumb_count = 0;

    // Mouse position based variables

    sf::Event event;
    sf::Vector2i mouse_curr, mouse_final = sf::Vector2i(0, 0);
    sf::Vector2i monster_mouse_curr, monster_mouse_final = sf::Vector2i(0, 0);
    sf::Vector2f mouse_final_float, monster_mouse_final_float;

    // Defining an array of boundary walls

    sf::Vector2f init_pos[] = { sf::Vector2f(0, 0), sf::Vector2f(640, 0), sf::Vector2f(640, 480), sf::Vector2f(0,480)};
    sf::Vector2f init_size = sf::Vector2f(640, 10);
    float init_rot[] = { 0, 90, 180, 270};
    Wall bound_walls[] = { Wall(init_pos[0], init_size, init_rot[0]), 
                           Wall(init_pos[1], init_size, init_rot[1]), 
                           Wall(init_pos[2], init_size, init_rot[2]), 
                           Wall(init_pos[3], init_size, init_rot[3]) 
                         };

    // Defining an array of obstacle walls

    sf::Vector2f wall_pos[] = { sf::Vector2f(200,0), sf::Vector2f(0, 200), sf::Vector2f(150, 200), sf::Vector2f(200, 160), sf::Vector2f(450, 160), sf::Vector2f(300, 160), sf::Vector2f(550, 160)};
    sf::Vector2f wall_size[] = { sf::Vector2f(200, 10), sf::Vector2f(150, 10), sf::Vector2f(250,10), sf::Vector2f(200,10), sf::Vector2f(200, 10), sf::Vector2f(260,10), sf::Vector2f(270,10)};
    Wall room_walls[] = { 
                          Wall(wall_pos[0], wall_size[0], 90), 
                          Wall(wall_pos[1], wall_size[1], 0), 
                          Wall(wall_pos[2], wall_size[2], 90), 
                          Wall(wall_pos[3], wall_size[3], 0), 
                          Wall(wall_pos[4], wall_size[4], 0), 
                          Wall(wall_pos[5], wall_size[5], 90), 
                          Wall(wall_pos[6], wall_size[6], 90)
                        };

    // Defining range of values where the sprite could move through

    int num_openings = 5;
    sf::Vector2f room_openings[] = { sf::Vector2f(165, 200), sf::Vector2f(150, 445), sf::Vector2f(245, 200), sf::Vector2f(290, 440), sf::Vector2f(540,445), sf::Vector2f(417, 160)};
    vector <Room> rooms  { 
                           Room( {sf::Vector2f(0, 0),sf::Vector2f(200,0),sf::Vector2f(200, 200),sf::Vector2f(0,200)}, 0, { sf::Vector2f(165, 200) }, { 2 }), 
                           Room( {sf::Vector2f(0,200),sf::Vector2f(140,200),sf::Vector2f(140,480),sf::Vector2f(0,480)}, 1, {sf::Vector2f(150, 445)}, { 2 } ),
                           Room( {sf::Vector2f(140,200), sf::Vector2f(300,200), sf::Vector2f(300,480), sf::Vector2f(140,480)}, 2, {sf::Vector2f(165, 200), sf::Vector2f(150, 445), sf::Vector2f(245, 200), sf::Vector2f(290, 440)}, { 1, 3, 5} ) ,  
                           Room( {sf::Vector2f(170,160), sf::Vector2f(300,160), sf::Vector2f(300,200), sf::Vector2f(170, 200)}, 3, {sf::Vector2f(245, 200) }, { 2 }), 
                           Room( {sf::Vector2f(170,0), sf::Vector2f(640,0), sf::Vector2f(640,160), sf::Vector2f(200,160)}, 4, {sf::Vector2f(417, 160)}, { 5 }),
                           Room( {sf::Vector2f(300,160), sf::Vector2f(540,160), sf::Vector2f(540,480), sf::Vector2f(140,480)}, 5, {sf::Vector2f(290, 440), sf::Vector2f(417, 160), sf::Vector2f(550,455)}, { 2, 6 }) , 
                           Room( {sf::Vector2f(540,160), sf::Vector2f(640,160), sf::Vector2f(640,480), sf::Vector2f(540,480)}, 6, {sf::Vector2f(550,455)}, { 5 })
                         };


    // Graph for environment
    vector <char> path, monster_path;
    Graph g;
    g.add_vertex(0, {{2, 1}});
    g.add_vertex(1, {{2, 1}});
    g.add_vertex(2, {{0, 1}, {1, 1}, {3, 1}, {5, 1}});
    g.add_vertex(3, {{2, 1}});
    g.add_vertex(4, {{5, 1}});
    g.add_vertex(5, {{2,1},{4,1},{6, 1}});
    g.add_vertex(6, {{5, 1}});

    // Flag set to true if sprite moves in same room
    // Flag set to true if mouse is clicked on a wall

    bool same_room = true, monster_same_room = true, wall_click = false, monster_wall_click = false;
    int current_room = 0, monster_current_room = 5, clicked_room = 0, monster_clicked_room = 5, ending_room = 0, monster_ending_room = 5;

    int previous_room = -1;

    // Variables for monster to follow a scripted path
    // {5, 4, 5, 6, 5, 2, 1, 2, 3, 2, 0, 2} 
    vector <int> scripted_path = {2, 0, 2, 3, 2, 1, 2, 5, 6, 5, 4};

    sf::Vector2f wander_point = sprite.get_position();
    sf::Vector2f monster_wander_point = monster.get_position();

    // bool seek_flag;

    struct DecisionTree* root = new DecisionTree(0);
    root->left = new DecisionTree(1);
    root->right = new DecisionTree(2);
    root->right->left = new DecisionTree(3);
    root->right->right = new DecisionTree(4);
    root->right->right->left = new DecisionTree(5);
    root->right->right->right = new DecisionTree(6);

    struct BehaviourTree* base = new BehaviourTree(0, 0);
    base->child.push_back(new BehaviourTree(1, 0));
    base->child[0]->parent = base;
    base->child.push_back(new BehaviourTree(2, 1));
    base->child[1]->parent = base;
    base->child.push_back(new BehaviourTree(3, 2));
    base->child[2]->parent = base;

    base->child[0]->child.push_back(new BehaviourTree(4, 0));
    base->child[0]->child[0]->parent = base->child[0];
    base->child[0]->child.push_back(new BehaviourTree(5, 1));
    base->child[0]->child[1]->parent = base->child[0];

    base->child[1]->child.push_back(new BehaviourTree(6,0));
    base->child[1]->child[0]->parent = base->child[1];
    base->child[1]->child.push_back(new BehaviourTree(7,1));
    base->child[1]->child[1]->parent = base->child[1];

    base->child[2]->child.push_back(new BehaviourTree(8,0));
    base->child[2]->child[0]->parent = base->child[2];
    base->child[2]->child.push_back(new BehaviourTree(9,1));
    base->child[2]->child[1]->parent = base->child[2];

    base->child[2]->child[1]->child.push_back(new BehaviourTree(10, 0));
    base->child[2]->child[1]->child[0]->parent = base->child[2]->child[1];
    base->child[2]->child[1]->child.push_back(new BehaviourTree(11, 1));
    base->child[2]->child[1]->child[1]->parent = base->child[2]->child[1];

    base->child[2]->child[1]->child[0]->child.push_back(new BehaviourTree(12, 0));
    base->child[2]->child[1]->child[0]->child[0]->parent = base->child[2]->child[1]->child[0];
    base->child[2]->child[1]->child[0]->child.push_back(new BehaviourTree(13, 1));
    base->child[2]->child[1]->child[0]->child[1]->parent = base->child[2]->child[1]->child[0];

    base->child[2]->child[1]->child[1]->child.push_back(new BehaviourTree(14, 0));
    base->child[2]->child[1]->child[1]->child[0]->parent = base->child[2]->child[1]->child[1];
    base->child[2]->child[1]->child[1]->child.push_back(new BehaviourTree(15, 1));
    base->child[2]->child[1]->child[1]->child[1]->parent = base->child[2]->child[1]->child[1];


    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            
            if (event.type == sf::Event::Closed)
            {
                std::cout<<"Closing Window";
                window.close();
            }  
        }

        window.clear(sf::Color::White);

        if(abs(monster.get_position().x - sprite.get_position().x ) < 10 && abs(monster.get_position().y - sprite.get_position().y ) < 10)
        {
            
            cout<<"Reset"<<endl;
            clock.restart();
            start_time = clock.getElapsedTime().asSeconds();
            
            monster_start_time = monster_clock.getElapsedTime().asSeconds();

            mouse_final = sf::Vector2i(0,0);
            monster_mouse_final = sf::Vector2i(360 ,300);
            sprite.set_position(sf::Vector2f(50.f, 50.f));
            monster.set_position(sf::Vector2f(360.f, 300.f));
            sprite.set_initial();
            monster.set_initial();
            sprite.reset_velocity();
            monster.reset_velocity();
            sprite.set_initial_rotation(0);
            sprite.set_target_rotation(0);
            monster.set_initial_rotation(0);
            monster.set_target_rotation(0);
            sprite.set_vel_flag();
            monster.set_vel_flag();
            path.clear();
            monster_path.clear();

            same_room = true;
            monster_same_room = true;

            current_room = 0;
            monster_current_room = 5;
            clicked_room = 0;
            monster_clicked_room = 5;
            ending_room = 0;
            monster_ending_room = 5;
            wall_click = false, monster_wall_click = false;

            // previous_room = -1;

            breadcrumbs.clear();
            
            monster_breadcrumbs.clear();

            crumb_count = 0;
            monster_crumb_count = 0;
            
            latest_crumb_pos = sf::Vector2f (10.f, 10.f);
            
            monster_latest_crumb_pos = sf::Vector2f (360.f, 300.f);

            scripted_path = {2, 0, 2, 3, 2, 1, 2, 5, 6, 5, 4};

            wander_point = sprite.get_position();
            monster_wander_point = monster.get_position();
            cout<<sprite.get_position().x<<" "<<sprite.get_position().y<<endl;
            cout<<monster.get_position().x<<" "<<monster.get_position().y<<endl;

            // Refreshing state of all nodes in behaviour tree

            base->refresh();

            base->child[0]->refresh();
            base->child[1]->refresh();
            base->child[2]->refresh();

            base->child[0]->child[0]->refresh();
            base->child[0]->child[1]->refresh();

            base->child[1]->child[0]->refresh();
            base->child[1]->child[1]->refresh();

            base->child[2]->child[0]->refresh();
            base->child[2]->child[1]->refresh();

            base->child[2]->child[1]->child[0]->refresh();
            base->child[2]->child[1]->child[1]->refresh();

            base->child[2]->child[1]->child[0]->child[0]->refresh();
            base->child[2]->child[1]->child[0]->child[1]->refresh();

            base->child[2]->child[1]->child[1]->child[0]->refresh();
            base->child[2]->child[1]->child[1]->child[1]->refresh();

        }

        DecisionTree* tree_node = root;

        while(tree_node->left != NULL)
        {
            if(tree_node->node_id == 0)
            {
                if( sprite.get_position().x - 3 <= wander_point.x && wander_point.x <= sprite.get_position().x + 3 && sprite.get_position().y - 3 <= wander_point.y && wander_point.y <= sprite.get_position().y + 3)
                {
                    tree_node = tree_node->right;
                }
                else
                {
                    tree_node = tree_node->left;
                }
            }
            else if(tree_node->node_id == 2)
            {
                if(clock.getElapsedTime().asSeconds() - start_time > 5)
                {
                    tree_node = tree_node->left;
                }
                else
                {
                    tree_node = tree_node->right;
                }
            }
            else if(tree_node->node_id == 4)
            {
                if(sprite.get_position().x - rooms[current_room].bottom_left.x <= 20 || sprite.get_position().y - rooms[current_room].top_left.y <= 20 || rooms[current_room].bottom_right.x - sprite.get_position().x <= 20 || rooms[current_room].bottom_left.y -  sprite.get_position().y <= 20)
                {
                    tree_node = tree_node->right;
                }
                else
                {
                    tree_node = tree_node->left;
                }
            }
        }
        
        if( tree_node->node_id == 3)
        {
            cout<<"Sprite moving to random room"<<endl;
            int rand_room = rand() % 7;
            while(rand_room == current_room) rand_room = rand() % 7;

            wander_point = rooms[rand_room].get_wander_point();
            start_time = 0;
            clock.restart();
        }
        else if(tree_node->node_id == 5)
        {
            cout<<"Sprite moving to random point within same room"<<endl;
            wander_point = rooms[current_room].get_wander_point();
        }
        else if(tree_node->node_id == 6 )
        {
            wander_point = rooms[current_room].room_centre;
            cout<<"Sprite moving to centre"<<endl;
        }

        path.erase(path.begin(), path.end());
        
        // Setting mouse pointer
        mouse_curr = mouse_final;
        mouse_final.x = wander_point.x;
        mouse_final.y = wander_point.y;
        mouse_final_float = sf::Vector2f(static_cast < float > (mouse_final.x), static_cast < float > (mouse_final.y) );
                
        // Logic that checks if wander_point is in same room

        if (mouse_final.x > rooms[current_room].top_left.x && mouse_final.x < rooms[current_room].top_right.x && mouse_final.y < rooms[current_room].bottom_left.y && mouse_final.y > rooms[current_room].top_right.y )
        {
            same_room = true;
            if(mouse_final.x != mouse_curr.x || mouse_final.y != mouse_curr.y)
            {
                sf::Vector2f vel = sprite.get_velocity();
                if (vel.x !=  0 && vel.y != 0) sprite.set_vel_flag(true);

                sprite.calculate_velocity(sprite.get_position(), mouse_final_float);
                sprite.set_initial();
                sprite.set_target(mouse_final_float);
                sprite.set_target_rotation(sprite.get_position(), mouse_final_float);
            }
        }
        else
        {
            same_room = false;
        }
                
        // If wander_point is in different room, perform pathfinding
                
        if(!same_room)
        {   
            for(int i = 0; i < 7;i++)
            {
                if(mouse_final.x > rooms[i].top_left.x && mouse_final.x < rooms[i].top_right.x && mouse_final.y > rooms[i].top_left.y && mouse_final.y < rooms[i].bottom_left.y)
                {
                    clicked_room = rooms[i].room_id;
                    break;
                }
            }


            for (int vertex : g.A_star(current_room, clicked_room) )
            {
                path.push_back(vertex);
            }
            ending_room = path[0];


            // Find intersecting entrance with nearby room

            bool exit_flag = false;
            for(int i = 0;i < 7;i++)
            {
                if( i == current_room) continue;
                for(auto entrance: rooms[i].room_entry)
                {
                    for(auto entrances: rooms[current_room].room_entry)
                    {
                        if(entrances == entrance) 
                        {
                            exit_flag = true;
                            if(mouse_final.x != mouse_curr.x || mouse_final.y != mouse_curr.y)
                            {
                                sf::Vector2f vel = sprite.get_velocity();
                                if (vel.x !=  0 && vel.y != 0) sprite.set_vel_flag(true);
                                sprite.calculate_velocity(sprite.get_position(), entrance);
                                sprite.set_initial();
                                sprite.set_target(entrance);
                                sprite.set_target_rotation(sprite.get_position(), entrance);   
                            }
                            break;
                        }
                    }
                    if(exit_flag) break;
                }
                if(exit_flag) break;
            }
        }
        

        bool exit_flag = false;
        sf::Vector2f final_entrance;

        if(sprite.get_change_room() && !same_room)
        {
            for(int i = path.size() - 1;i >=0;i--)
            {
                for(auto entrance: rooms[path[i]].room_entry)
                {
                    for(auto entrances: rooms[current_room].room_entry)
                    {
                        if(entrances == entrance) 
                        {
                            exit_flag = true;
                            final_entrance = entrance;
                            sprite.set_target(entrance);
                            current_room = path.back();
                            path.pop_back();
                            sprite.set_change_room(false);
                            break;
                        }
                    }
                    if(exit_flag) break;
                }
                if(exit_flag) break;
            }
            
        }

        if(!path.size() && sprite.get_change_room())
        {
            sprite.set_target(mouse_final_float);
        }

        // Movement and Breadcrumb logic using double ended queue
        if(sprite.getInitial() != sprite.getTarget() )
        {
            sprite.move();

            if( sqrt( pow(sprite.get_position().x - latest_crumb_pos.x, 2.0) + pow(sprite.get_position().y - latest_crumb_pos.y, 2.0 ))  > 50 ) 
            {
                crumb c(false);
                c.drop(latest_crumb_pos); 
                latest_crumb_pos = sprite.get_position();

                if ( crumb_count > CRUMB_SIZE ){ breadcrumbs.pop_front(); breadcrumbs.push_back(c); }
                else breadcrumbs.push_back(c);

                crumb_count++;
            }
            
        }

        // Behaviour Tree Logic

        int action_choice = 0;

        BehaviourTree* b_node = base;

        while(!action_choice)
        {
            if(b_node->node_id == 0)
            {
               if(b_node->prev_node == -1)
               {
                   b_node->prev_node = 0;
                   b_node->prev_node_finish = false;
                   b_node = b_node->child[0];
               }
               else if (b_node->prev_node_finish == false)
               {
                   b_node = b_node->child[b_node->prev_node];
               }
               else if (b_node->prev_node_finish == true && b_node->prev_node <= b_node->child.size() - 2)
               {
                   b_node->prev_node+=1;
                   b_node = b_node->child[b_node->prev_node];
               }
               else
               {
                    base->refresh();

                    base->child[0]->refresh();
                    base->child[1]->refresh();
                    base->child[2]->refresh();

                    base->child[0]->child[0]->refresh();
                    base->child[0]->child[1]->refresh();

                    base->child[1]->child[0]->refresh();
                    base->child[1]->child[1]->refresh();

                    base->child[2]->child[0]->refresh();
                    base->child[2]->child[1]->refresh();

                    base->child[2]->child[1]->child[0]->refresh();
                    base->child[2]->child[1]->child[1]->refresh();

                    base->child[2]->child[1]->child[0]->child[0]->refresh();
                    base->child[2]->child[1]->child[0]->child[1]->refresh();

                    base->child[2]->child[1]->child[1]->child[0]->refresh();
                    base->child[2]->child[1]->child[1]->child[1]->refresh();
               }
               
               
            }
            else if(b_node->node_id == 1)
            {
                
                int check_nodes = 0;
                bool result_node = true;

                while(result_node && !action_choice)
                {
                    if(b_node->prev_node == -1 || b_node->prev_node <= b_node->child.size() - 2)
                    {
                        b_node->prev_node+=1;
                        b_node = b_node->child[b_node->prev_node];
                    }
                    else
                    {
                        result_node = false;
                        break;
                    }

                    if(b_node->node_id == 4)
                    {
                        if(!( monster.get_position().x - 3 <= monster_wander_point.x && monster_wander_point.x <= monster.get_position().x + 3 && monster.get_position().y - 3 <= monster_wander_point.y && monster_wander_point.y <= monster.get_position().y + 3))
                        {
                            result_node = true;
                            b_node = b_node->parent;
                            b_node->prev_node_finish = true;
                            b_node->prev_output = true;
                        }
                        else
                        {
                            result_node = false;
                            b_node = b_node->parent;
                            b_node->prev_node_finish = true;
                            b_node->prev_output = false;

                            b_node = b_node->parent;
                            b_node->prev_node_finish = true;
                            b_node->prev_output = false;
                            break;
                        }
                    }
                    else if (b_node->node_id == 5)
                    {
                        action_choice = 1;
                        base->refresh();

                        base->child[0]->refresh();
                        base->child[1]->refresh();
                        base->child[2]->refresh();

                        base->child[0]->child[0]->refresh();
                        base->child[0]->child[1]->refresh();

                        base->child[1]->child[0]->refresh();
                        base->child[1]->child[1]->refresh();

                        base->child[2]->child[0]->refresh();
                        base->child[2]->child[1]->refresh();

                        base->child[2]->child[1]->child[0]->refresh();
                        base->child[2]->child[1]->child[1]->refresh();

                        base->child[2]->child[1]->child[0]->child[0]->refresh();
                        base->child[2]->child[1]->child[0]->child[1]->refresh();

                        base->child[2]->child[1]->child[1]->child[0]->refresh();
                        base->child[2]->child[1]->child[1]->child[1]->refresh();
                    }
                    
                }
            }
            else if (b_node->node_id == 2)
            {
                int check_nodes = 0;
                bool result_node = true;

                while(result_node && !action_choice)
                {
                    if(b_node->prev_node == -1 || b_node->prev_node <= b_node->child.size() - 2)
                    {
                        b_node->prev_node+=1;
                        b_node = b_node->child[b_node->prev_node];
                    }
                    else
                    {
                        result_node = false;
                        break;
                    }

                    if(b_node->node_id == 6)
                    {
                        if(current_room == monster_current_room)
                        {

                            result_node = true;
                            b_node = b_node->parent;
                            b_node->prev_node_finish = true;
                            b_node->prev_output = true;
                        }
                        else
                        {
                            result_node = false;
                            b_node = b_node->parent;
                            b_node->prev_node_finish = true;
                            b_node->prev_output = false;

                            b_node = b_node->parent;
                            b_node->prev_node_finish = true;
                            b_node->prev_output = false;
                            break;
                        }
                    }
                    else if (b_node->node_id == 7)
                    {
                        action_choice = 2;
                        base->refresh();

                        base->child[0]->refresh();
                        base->child[1]->refresh();
                        base->child[2]->refresh();

                        base->child[0]->child[0]->refresh();
                        base->child[0]->child[1]->refresh();

                        base->child[1]->child[0]->refresh();
                        base->child[1]->child[1]->refresh();

                        base->child[2]->child[0]->refresh();
                        base->child[2]->child[1]->refresh();

                        base->child[2]->child[1]->child[0]->refresh();
                        base->child[2]->child[1]->child[1]->refresh();

                        base->child[2]->child[1]->child[0]->child[0]->refresh();
                        base->child[2]->child[1]->child[0]->child[1]->refresh();

                        base->child[2]->child[1]->child[1]->child[0]->refresh();
                        base->child[2]->child[1]->child[1]->child[1]->refresh();
                    }
                    
                }
            }
            else if (b_node->node_id == 3)
            {
                int check_nodes = 0;
                bool result_node = true;

                while(result_node && !action_choice)
                {
                    if(b_node->prev_node == -1 || b_node->prev_node <= b_node->child.size() - 2)
                    {
                        b_node->prev_node+=1;
                        b_node = b_node->child[b_node->prev_node];
                    }
                    else
                    {
                        result_node = false;
                        break;
                    }

                    if(b_node->node_id == 8)
                    {
                        if(current_room != monster_current_room)
                        {
                            result_node = true;
                            b_node = b_node->parent;
                            b_node->prev_node_finish = true;
                            b_node->prev_output = true;
                        }
                        else
                        {
                            result_node = false;
                            b_node = b_node->parent;
                            b_node->prev_node_finish = true;
                            b_node->prev_output = false;

                            b_node = b_node->parent;
                            b_node->prev_node_finish = true;
                            b_node->prev_output = false;
                            break;
                        }
                    }
                    else if (b_node->node_id == 9)
                    {
                        bool selector_flag = true;
                        while(selector_flag && !action_choice)
                        {
                            if(b_node->prev_node == -1 || b_node->prev_node <= b_node->child.size() - 2)
                            {
                                b_node->prev_node+=1;
                                b_node = b_node->child[b_node->prev_node];
                            }
                            else
                            {
                                selector_flag = false;
                                break;
                            }

                            if(b_node->node_id == 10)
                            {
                                bool sequence_flag = true;
                                while(sequence_flag && !action_choice)
                                {
                                    if(b_node->prev_node == -1 || b_node->prev_node <= b_node->child.size() - 2)
                                    {
                                        b_node->prev_node+=1;
                                        b_node = b_node->child[b_node->prev_node];
                                    }
                                    else
                                    {
                                        sequence_flag = false;
                                        break;
                                    }

                                    if(b_node->node_id == 12)
                                    {
                                        if(monster_clock.getElapsedTime().asSeconds() - monster_start_time < 10)
                                        {
                                            sequence_flag = true;
                                            b_node = b_node->parent;
                                            b_node->prev_node_finish = true;
                                            b_node->prev_output = true;
                                        }
                                        else
                                        {
                                            sequence_flag = false;
                                            b_node = b_node->parent;
                                            b_node->prev_node_finish = true;
                                            b_node->prev_output = false;

                                            b_node = b_node->parent;
                                            b_node->prev_node_finish = true;
                                            b_node->prev_output = false;
                                            break;
                                        }

                                    }
                                    else if(b_node->node_id == 13)
                                    {
                                        action_choice = 3;
                                        base->refresh();

                                        base->child[0]->refresh();
                                        base->child[1]->refresh();
                                        base->child[2]->refresh();

                                        base->child[0]->child[0]->refresh();
                                        base->child[0]->child[1]->refresh();

                                        base->child[1]->child[0]->refresh();
                                        base->child[1]->child[1]->refresh();

                                        base->child[2]->child[0]->refresh();
                                        base->child[2]->child[1]->refresh();

                                        base->child[2]->child[1]->child[0]->refresh();
                                        base->child[2]->child[1]->child[1]->refresh();

                                        base->child[2]->child[1]->child[0]->child[0]->refresh();
                                        base->child[2]->child[1]->child[0]->child[1]->refresh();

                                        base->child[2]->child[1]->child[1]->child[0]->refresh();
                                        base->child[2]->child[1]->child[1]->child[1]->refresh();   
                                    }
                                }
                            }
                            else if(b_node->node_id == 11)
                            {
                                bool sequence_flag = true;
                                while(sequence_flag && !action_choice)
                                {
                                    if(b_node->prev_node == -1 || b_node->prev_node <= b_node->child.size() - 2)
                                    {
                                        b_node->prev_node+=1;
                                        b_node = b_node->child[b_node->prev_node];
                                    }
                                    else
                                    {
                                        sequence_flag = false;
                                        break;
                                    }

                                    if(b_node->node_id == 14)
                                    {
                                        if(monster_clock.getElapsedTime().asSeconds() - monster_start_time > 5)
                                        {
                                            sequence_flag = true;
                                            b_node = b_node->parent;
                                            b_node->prev_node_finish = true;
                                            b_node->prev_output = true;
                                        }
                                        else
                                        {
                                            sequence_flag = false;
                                            b_node = b_node->parent;
                                            b_node->prev_node_finish = true;
                                            b_node->prev_output = false;

                                            b_node = b_node->parent;
                                            b_node->prev_node_finish = true;
                                            b_node->prev_output = false;
                                            break;
                                        }

                                    }
                                    else if(b_node->node_id == 15)
                                    {
                                        action_choice = 4;
                                        base->refresh();

                                        base->child[0]->refresh();
                                        base->child[1]->refresh();
                                        base->child[2]->refresh();

                                        base->child[0]->child[0]->refresh();
                                        base->child[0]->child[1]->refresh();

                                        base->child[1]->child[0]->refresh();
                                        base->child[1]->child[1]->refresh();

                                        base->child[2]->child[0]->refresh();
                                        base->child[2]->child[1]->refresh();

                                        base->child[2]->child[1]->child[0]->refresh();
                                        base->child[2]->child[1]->child[1]->refresh();

                                        base->child[2]->child[1]->child[0]->child[0]->refresh();
                                        base->child[2]->child[1]->child[0]->child[1]->refresh();

                                        base->child[2]->child[1]->child[1]->child[0]->refresh();
                                        base->child[2]->child[1]->child[1]->child[1]->refresh();   
                                    }
                                }
                            }
                            
                        }
                    }
                    
                }
            }
            
        }

        if(action_choice != 1)
        {
            if(action_choice == 2)
            {
                monster_wander_point = sprite.get_position();
                cout<<"Monster seeking sprite"<<endl;
            }
            else if(action_choice == 3)
            {
                monster_wander_point = rooms[monster_current_room].get_wander_point();
                cout<<"Monster wanders between same room"<<endl;
            }
            else if(action_choice == 4)
            {
                cout<<"Monster wanders to next room"<<endl;
                int next_room = scripted_path.back();
                scripted_path.pop_back();
                scripted_path.insert(scripted_path.begin(), next_room);

                monster_wander_point = rooms[next_room].get_wander_point();
                monster_start_time = 0;
                monster_clock.restart();
            }

            monster_path.erase(monster_path.begin(), monster_path.end());
            
            monster_mouse_curr = monster_mouse_final;
            monster_mouse_final.x = monster_wander_point.x;
            monster_mouse_final.y = monster_wander_point.y;
            monster_mouse_final_float = sf::Vector2f(static_cast < float > (monster_mouse_final.x), static_cast < float > (monster_mouse_final.y) );
            
            // Logic that checks if mouse_click is in same room
            if (monster_mouse_final.x > rooms[monster_current_room].top_left.x && monster_mouse_final.x < rooms[monster_current_room].top_right.x && monster_mouse_final.y < rooms[monster_current_room].bottom_left.y && monster_mouse_final.y > rooms[monster_current_room].top_right.y )
            {
                monster_same_room = true;
                if(monster_mouse_final.x != monster_mouse_curr.x || monster_mouse_final.y != monster_mouse_curr.y)
                {
                    sf::Vector2f vel = monster.get_velocity();
                    if (vel.x !=  0 && vel.y != 0) monster.set_vel_flag(true);

                    monster.calculate_velocity(monster.get_position(), monster_mouse_final_float);
                    monster.set_initial();
                    monster.set_target(monster_mouse_final_float);
                    monster.set_target_rotation(monster.get_position(), monster_mouse_final_float);
                }
            }
            else
            {
                monster_same_room = false;
            }
            
            // If Mouse click is in different room, perform pathfinding
            
            if(!monster_same_room)
            {   
                for(int i = 0; i < 7;i++)
                {
                    if(monster_mouse_final.x > rooms[i].top_left.x && monster_mouse_final.x < rooms[i].top_right.x && monster_mouse_final.y > rooms[i].top_left.y && monster_mouse_final.y < rooms[i].bottom_left.y)
                    {
                        monster_clicked_room = rooms[i].room_id;
                        break;
                    }
                }


                if(monster_clicked_room != monster_current_room)
                {
                    for (int vertex : g.A_star(monster_current_room, monster_clicked_room) )
                    {
                        monster_path.push_back(vertex);
                    }
                    monster_ending_room = monster_path[0];


                    // Find intersecting entrance with nearby room

                    bool exit_flag = false;
                    for(int i = 0;i < 7;i++)
                    {
                        if( i == monster_current_room) continue;
                        for(auto entrance: rooms[i].room_entry)
                        {
                            for(auto entrances: rooms[monster_current_room].room_entry)
                            {
                                if(entrances == entrance) 
                                {
                                    exit_flag = true;
                                    if(monster_mouse_final.x != monster_mouse_curr.x || monster_mouse_final.y != monster_mouse_curr.y)
                                    {
                                        sf::Vector2f vel = monster.get_velocity();
                                        if (vel.x !=  0 && vel.y != 0) monster.set_vel_flag(true);
                                        monster.calculate_velocity(monster.get_position(), entrance);
                                        monster.set_initial();
                                        monster.set_target(entrance);
                                        monster.set_target_rotation(monster.get_position(), entrance);   
                                    }
                                    break;
                                }
                            }
                            if(exit_flag) break;
                        }
                        if(exit_flag) break;
                    }
                }
                else
                {
                    monster_same_room = true;
                }
            }
        }
        
        sf::Vector2f monster_final_entrance;
        bool monster_exit_flag = false;

        if(monster.get_change_room() && !monster_same_room)
        {
            for(int i = monster_path.size() - 1;i >=0;i--)
            {
                for(auto entrance: rooms[monster_path[i]].room_entry)
                {
                    for(auto entrances: rooms[monster_current_room].room_entry)
                    {
                        if(entrances == entrance) 
                        {
                            monster_exit_flag = true;
                            monster_final_entrance = entrance;
                            monster.set_target(entrance);
                            monster_current_room = monster_path.back();
                            monster_path.pop_back();
                            monster.set_change_room(false);
                            break;
                        }
                    }
                    if(monster_exit_flag) break;
                }
                if(monster_exit_flag) break;
            }
            
        }

        if(!monster_path.size() && monster.get_change_room())
        {
            monster.set_target(monster_mouse_final_float);
        }

        if(monster.getInitial() != monster.getTarget() )
        {
            monster.move();

            if( sqrt( pow(monster.get_position().x - monster_latest_crumb_pos.x, 2.0) + pow(monster.get_position().y - monster_latest_crumb_pos.y, 2.0 ))  > 50 ) 
            {
                crumb c(true);
                c.drop(monster_latest_crumb_pos); 
                monster_latest_crumb_pos = monster.get_position();

                if ( crumb_count > CRUMB_SIZE ){ monster_breadcrumbs.pop_front(); monster_breadcrumbs.push_back(c); }
                else monster_breadcrumbs.push_back(c);

                crumb_count++;
            }
            
        }

        // Drawing walls and boundaries
        for(int i = 0; i < NUM_BOUND_WALLS; i++)
        {
            bound_walls[i].draw(&window);
        }

        for(int i = 0; i < NUM_ROOM_WALLS;i++)
        {
            room_walls[i].draw(&window);
        }
        
        // Drawing the sprite
        window.draw(monster.obj);
        window.draw(sprite.obj);

        // Draw breadcrumbs
        for(auto x : breadcrumbs) x.draw(&window);
        for(auto x : monster_breadcrumbs) x.draw(&window);

        // Displaying the window
        window.display();
    }

    return 0;
}