#include<SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#include<random>
using namespace std;

// Created 4 virtual classes of the steering behaviour class
class SteeringBehaviour
{
    public:
        void virtual calculate_velocity(sf::Vector2f curr, sf::Vector2f target) = 0;
        sf::Vector2f virtual get_position() = 0;
        void virtual set_position(sf::Vector2i position) = 0;
};

// Position class
class Position : SteeringBehaviour
{
        public:
        sf::Vector2f target, initial;
        sf::Vector2f position;
   
};

// Velocity class
class Velocity : SteeringBehaviour
{
    public:
        float max_speed = 3;
        sf::Vector2f velocity;
};

// Orientation and Rotation class (no use since we do not have to rotate the sprite)
class Orientation : SteeringBehaviour{};
class Rotation: SteeringBehaviour{};

class Sprite :Velocity,  Position
{

    private:
        sf::Texture texture;
    public:
        sf::CircleShape obj;

    Sprite()
    {
        this->obj = obj;
        this->obj.setRadius(15.0f);
        this->velocity = sf::Vector2f(0.0f, 0.0f);
        this->position = obj.getPosition();
        this->obj.setFillColor(sf::Color(0, 0, 0));
        this->obj.setPosition(0,0);
    } 
    // Getters and setters
    sf::Vector2f get_position()
    {
        return obj.getPosition();
    }

    void set_position(sf::Vector2i position)
    {
        this->position.x = position.x;
        this->position.y = position.y;
    }

    sf::Vector2f get_velocity()
    {
        return this->velocity;
    }

    void set_velocity(sf::Vector2f velocity)
    {
        this->velocity = velocity;
    }

    sf::Vector2f getTarget()
    {
        return this->target;
    }

    sf::Vector2f getInitial()
    {
        return this->initial;
    }

    sf::Vector2f checkBorder(sf::Vector2f location)
    {
        if(location.x < 0) location.x = 640.0f;
        if(location.x > 640) location.x = 0.0f;
        if(location.y > 480) location.y = 0.0f;
        if(location.y < 0) location.y = 480.0f;
        return location;
    }

    void move()
    {
        sf::Vector2f location = get_position() + this->velocity;
        
        location = checkBorder(location);

        if ((velocity.x >0 && location.x <= this->target.x) || (velocity.x < 0 && location.x >=this->target.x) || (location.y<=this->target.y && velocity.y >0) || (velocity.y <0 && location.y >= this->target.y ))
        {
            obj.setPosition(location);
            this->initial += this->velocity;
        }
    }

    void normalize(float req_speed)
    {
        this->velocity.x  /= req_speed;
        this->velocity.y /= req_speed; 
    }

    void calculate_velocity(sf::Vector2f curr, sf::Vector2f final)
    {
        this->velocity = final - curr;
        float req_speed = sqrt( pow(velocity.x, 2.0) + pow(velocity.y, 2.0) );

        if (req_speed > max_speed)
        {
            normalize(req_speed);
            this->velocity *= max_speed;
        }
        
        this->target = final;
    }
    
    void reset_initial(bool var)
    {
        if(var)
        {
            if(initial.x < 0.0f)
                this->initial.x = 640.0f;
            else
                this->initial.x = 0.0f;
        }
        else
        {
            if(initial.y < 0.0f)
                this->initial.y = 480.0f;
            else
                this->initial.y = 0.0f;
        }
    }

};


int main()
{
    // Creating the window and initial variables
    sf::RenderWindow window(sf::VideoMode(640, 480), "HW1 Steering Behaviour");
    Sprite sprite;
    sf::Event event;

    sf::Vector2i mouse_curr, mouse_final, mouse_bound;

    // Flag to know whether mouse is in screen or not
    bool mouse_out = true;


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

        mouse_bound = sf::Mouse::getPosition(window);
        
        if(mouse_out && mouse_bound.x >=0 && mouse_bound.x <= 640 && mouse_bound.y >=0 && mouse_bound.y <= 480)
        {
            mouse_curr = mouse_bound;
            mouse_out = false;
        }
        else if( mouse_bound.x < 0 || mouse_bound.x > 640 || mouse_bound.y < 0 || mouse_bound.y > 480 ) mouse_out = true;
        else
        {
            if(event.type == sf::Event::MouseMoved)
            {
                mouse_curr = mouse_final;
                mouse_final.x = event.mouseMove.x;
                mouse_final.y = event.mouseMove.y;

                sf::Vector2f mouse_offset = sf::Vector2f(static_cast < float >((mouse_final.x - mouse_curr.x)), static_cast < float >((mouse_final.y - mouse_curr.y)));

                if(mouse_offset.x != 0 || mouse_offset.y != 0)
                    sprite.calculate_velocity(sprite.get_position(), mouse_offset + sprite.get_position());
            }
        }

        if(sprite.getInitial().x < 0.0f || sprite.getInitial().x > 640.0f) sprite.reset_initial(true);

        else if(sprite.getInitial().y < 0.0f || sprite.getInitial().y > 480.0f) sprite.reset_initial(false);

        else if(sprite.getInitial() != sprite.getTarget()) sprite.move();
        
        // // Drawing the sprite
        window.draw(sprite.obj);

        // // Displaying the window
        window.display();    

    }

    return 0;
}