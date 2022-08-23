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
        void virtual set_position(sf::Vector2i position) = 0;

};

class Position : SteeringBehaviour
{
        public:
        float radiusDeceleration = 100.0, radiusSatisfaction = 1.0, timeToTargetVelocity = 10.0;
        sf::Vector2f target, initial;
        sf::Vector2f position;

        void set_position(sf::Vector2i position)
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

    Sprite()
    {
        this->obj = obj;
        this->obj.setRadius(7.0f);
        this->velocity = sf::Vector2f(0.0f, 0.0f);
        this->position = obj.getPosition();
        this->obj.setFillColor(sf::Color(0, 0, 0));
        this->obj.setPosition(50,50);
    }

    void set_position(sf::Vector2i position)
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
        if(abs(targetRotation - this->rotation) > radiusSatisfaction)
        {
            calculate_rotation();
            obj.setRotation(this->rotation);
            this->initialRotation = this->rotation;
        }
        
        sf::Vector2f location = get_position() + this->velocity;
        obj.setPosition(location);
        this->initial += this->velocity;
        
    }

    void calculate_velocity(sf::Vector2f curr, sf::Vector2f final)
    {
        float goalSpeed, goalSpeed1;
        
        sf::Vector2f new_distance = final - curr;

        float req_speed = sqrt( pow(new_distance.x, 2.0) + pow(new_distance.y, 2.0) );


        if(req_speed < radiusSatisfaction) {if (vel_flag) set_vel_flag(); goalSpeed = 0; change_room = true;}
        else if(req_speed > radiusDeceleration) goalSpeed = MAX_SPEED;
        else goalSpeed = MAX_SPEED * req_speed / radiusDeceleration;

        
        goalVelocity = new_distance;
        
        goalVelocity = normalize(goalVelocity);

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

            newGoalVelocity = normalize(newGoalVelocity);
            
            newGoalVelocity *= goalSpeed;
            goalVelocity *= goalSpeed;
            this->velocity = (goalVelocity - newGoalVelocity) / timeToTargetVelocity; 
            this->target -= this->velocity;
            set_target_rotation(curr,this->target);
        }
        else
        {
            goalVelocity *= goalSpeed;
            this->velocity = (goalVelocity - this->velocity) / timeToTargetVelocity; 
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

        crumb()
        {
            this->setRadius(5.0f);
            srand(time(NULL));
            this->setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
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
        vector <sf::Vector2f> room_entry;
        int num_entries;

        Room(vector <sf::Vector2f> coordinates, int room_id, vector <sf::Vector2f> room_entry)
        {
            this->top_left = coordinates[0];
            this->top_right = coordinates[1];
            this->bottom_right = coordinates[2];
            this->bottom_left = coordinates[3];
            this->room_id = room_id;
            for(auto re: room_entry)
                this->room_entry.push_back(re);

            this->num_entries = room_entry.size();
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

        vector<int> dijkstra(int first, int final)
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


int main()
{
    // Creating the window and intial variables

    sf::RenderWindow window(sf::VideoMode(640, 480), "HW3 Part 4");
    Sprite sprite;  
    sf::Texture texture;
    srand(time(0));

    // Breadcrumbs

    deque <crumb> breadcrumbs;
    sf::Vector2f latest_crumb_pos = sf::Vector2f (0.f, 0.f);
    int crumb_count = 0;

    // Mouse position based variables

    sf::Event event;
    sf::Vector2i mouse_curr, mouse_final = sf::Vector2i(0, 0);
    sf::Vector2f mouse_final_float;

    bool mouse_out = true;

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

    // Defining another data structure that stores wall information

    // vector <vector <sf::Vector2f> > wall_info { {sf::Vector2f(145,200), sf::Vector2f(144,450) }, { sf::Vector2f(295,420), sf::Vector2f(295,160) }, {sf::Vector2f(10,200), sf::Vector2f(140,200) }, { sf::Vector2f(190,10), sf::Vector2f(195,200) }, { sf::Vector2f(194,160), sf::Vector2f(400,160)}, { sf::Vector2f(450,160), sf::Vector2f(620,160) }, { sf::Vector2f(540,160), sf::Vector2f(540,430)} };

    // Defining range of values where the sprite could move through

    int num_openings = 5;
    sf::Vector2f room_openings[] = { sf::Vector2f(165, 195), sf::Vector2f(137, 452), sf::Vector2f(245, 200), sf::Vector2f(290, 440), sf::Vector2f(540,445), sf::Vector2f(417, 160)};
    vector <Room> rooms  { 
                           Room( {sf::Vector2f(10,10),sf::Vector2f(190,5),sf::Vector2f(190,195),sf::Vector2f(10,210)}, 0, { sf::Vector2f(165, 195) }), 
                           Room( {sf::Vector2f(10,210),sf::Vector2f(140,210),sf::Vector2f(140,445),sf::Vector2f(5,465)}, 1, {sf::Vector2f(150, 455)} ),
                           Room( {sf::Vector2f(145,210), sf::Vector2f(290,210), sf::Vector2f(290,465), sf::Vector2f(150,460)}, 2, {sf::Vector2f(165, 195), sf::Vector2f(150, 455), sf::Vector2f(245, 200), sf::Vector2f(290, 440)} ) ,  
                           Room( {sf::Vector2f(195,165), sf::Vector2f(295,165), sf::Vector2f(295,195), sf::Vector2f(195,195)}, 3, {sf::Vector2f(245, 200) }), 
                           Room( {sf::Vector2f(200,10), sf::Vector2f(625,10), sf::Vector2f(625,150), sf::Vector2f(200,150)}, 4, {sf::Vector2f(417, 160)}),
                           Room( {sf::Vector2f(295,175), sf::Vector2f(540,175), sf::Vector2f(540,465), sf::Vector2f(295,465)}, 5, {sf::Vector2f(290, 440), sf::Vector2f(417, 160), sf::Vector2f(550,455)} ) , 
                           Room( {sf::Vector2f(550,175), sf::Vector2f(625,175), sf::Vector2f(625,465), sf::Vector2f(550,465)}, 6, {sf::Vector2f(550,455)})
                         };

    // sf::CircleShape circles[] = { sf::CircleShape(7,30), sf::CircleShape(7,30), sf::CircleShape(7,30), sf::CircleShape(7,30), sf::CircleShape(7,30), sf::CircleShape(7,30)};

    // Graph for environment
    vector <char> path;
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
    bool same_room = true, wall_click = false;
    int current_room = 0, clicked_room = 0, ending_room = 0;
    bool clicked_on_wall = false;
    bool err_flag = false;

    // for(int i = 0; i < num_openings;i++)
    // {
    //     circles[i].setPosition(room_openings[i]);
    //     circles[i].setFillColor(sf::Color::Black);
    // }

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

        // Event triggered every time the mouse is pressed
        if(event.type ==  sf::Event::MouseButtonPressed)
        {
                // Erasing path vector 
                path.erase(path.begin(), path.end());
                // Setting mouse pointer
                mouse_curr = mouse_final;
                mouse_final.x = event.mouseButton.x;
                mouse_final.y = event.mouseButton.y;  
                mouse_final_float = sf::Vector2f(static_cast < float > (mouse_final.x), static_cast < float > (mouse_final.y) );
                
                // Logic that checks if mouse_click is in same room

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
                
                // If Mouse click is in different room, perform pathfinding
                
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


                    for (int vertex : g.dijkstra(current_room, clicked_room) )
                    {
                        // cout<<vertex<<endl;
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
                            cout<<rooms[path[i]].room_id<<" "<<rooms[current_room].room_id<<endl;
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
                crumb c;
                c.drop(latest_crumb_pos); 
                latest_crumb_pos = sprite.get_position();

                if ( crumb_count > CRUMB_SIZE ){ breadcrumbs.pop_front(); breadcrumbs.push_back(c); }
                else breadcrumbs.push_back(c);

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
        window.draw(sprite.obj);

        // Draw breadcrumbs
        for(auto x : breadcrumbs) x.draw(&window);

        // Displaying the window
        window.display();
    }


    return 0;
}