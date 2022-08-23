#include<SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#include<deque>
#include<time.h>
using namespace std;

// Constants
#define PI 3.14
#define ROT_THR 1
#define CRUMB_SIZE 5
#define MAX_SPEED 2

// Created 4 virtual classes of the steering behaviour class

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
        float radiusDeceleration = 0, radiusSatisfaction = 2.0, timeToTargetVelocity = 10.0;
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
        sf::CircleShape obj;
        bool vel_flag = false;

    Sprite()
    {
        this->obj = obj;
        this->obj.setRadius(15.0f);
        this->velocity = sf::Vector2f(0.0f, 0.0f);
        this->position = obj.getPosition();
        this->obj.setFillColor(sf::Color(0, 0, 0));
        this->obj.setPosition(0,0);
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


        if(req_speed < radiusSatisfaction) {if (vel_flag) set_vel_flag(); goalSpeed = 0;}
        else if(req_speed > radiusDeceleration) goalSpeed = MAX_SPEED;
        else goalSpeed = MAX_SPEED * req_speed / radiusDeceleration;

        goalVelocity = new_distance;
        goalVelocity = normalize(goalVelocity);

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
        float reqRotation1= targetRotation - this->rotation;

        if (min(abs(reqRotation1),abs(360+reqRotation1))==abs(reqRotation1)) reqRotation=reqRotation1;
        else reqRotation=mod_360(360+reqRotation1);


        if( abs(reqRotation) < rotationSatisfaction) goalRotation = 0;
        else if(abs(reqRotation) > rotationDeceleration) 
        {
            if(reqRotation>0) {goalRotation = maxRotation;}
            else goalRotation = -maxRotation;
        }
        else  goalRotation = maxRotation * reqRotation / rotationDeceleration;


        goalRotation /= timeToTargetRotation;

        if (goalRotation < 0) goalRotation = max(goalRotation, -maxRotation);
        else goalRotation = min(goalRotation, maxRotation);
        
        int rr = goalRotation;
        float decimal = goalRotation - rr;
        int int_rot = this->rotation;
        float rot_decimal = this->rotation - int_rot;
        this->rotation =  (int_rot + rr)%360 + decimal + rot_decimal;

    }
    
};

class crumb : sf::CircleShape
{
    public:

        crumb()
        {
            this->setRadius(10.f);
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


int main()
{
    // Creating the window and intial variables
    sf::RenderWindow window(sf::VideoMode(640, 480), "HW2 Part 2");
    Sprite sprite;
    sf::Texture texture;
    srand(time(0));

    // Breadcrumbs
    deque <crumb> breadcrumbs;
    sf::Vector2f latest_crumb_pos = sf::Vector2f (0.f, 0.f);
    int crumb_count = 0;

    sf::Event event;

    // Point to wander to
    sf::Vector2f wander_point = sf::Vector2f(0,0);

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

        // Clears the window with white color
        window.clear(sf::Color::White);

        if (sprite.get_position().x - 2 <= wander_point.x && wander_point.x <= sprite.get_position().x + 2 && sprite.get_position().y - 2 <= wander_point.y && wander_point.y <= sprite.get_position().y + 2)
        {
            wander_point = sf::Vector2f(rand() % 640, rand() % 480);
            sprite.calculate_velocity(sprite.get_position(), wander_point);
            sprite.set_initial();
            sprite.set_target(wander_point);
            sprite.set_target_rotation(sprite.get_position(), wander_point);
            
        }

        if(sprite.getInitial() != sprite.getTarget())
        {
            sprite.move();

            if( sqrt( pow(sprite.get_position().x - latest_crumb_pos.x, 2.0) + pow(sprite.get_position().y - latest_crumb_pos.y, 2.0 ))  > 50 ) 
            {
                crumb c;
                c.drop(latest_crumb_pos); 
                latest_crumb_pos = sprite.get_position();

                if ( crumb_count > CRUMB_SIZE ){  breadcrumbs.pop_front(); breadcrumbs.push_back(c); }
                else{ breadcrumbs.push_back(c); }

                crumb_count++;
            }
            
        }
        
        // Drawing breadcrumbs
        for(auto x : breadcrumbs) x.draw(&window);

        // // Drawing the sprite
        window.draw(sprite.obj);

        // // Displaying the window
        window.display();
       
    }

    return 0;
}