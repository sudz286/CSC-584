#include<SFML/Graphics.hpp>
#include<iostream>
#define XSPEED 0.133
#define YSPEED 0.10

class Sprite
{

    private:
        sf::Vector2f position;
        sf::Texture texture;
    public:
        sf::Sprite obj;

    Sprite()
    {
        this->obj = obj;
        this->position = obj.getPosition();

        // Loading image from file
        if (!texture.loadFromFile("../Images/boid.png", sf::IntRect(0, 0, 930, 627)))
        {
            std::cout<<"Error, unable to load sprite image";
            exit (0);
        }
        this->obj.setTexture(texture);
        this->obj.setPosition(0,0);
        this->obj.scale(0.04,0.04);
    }   

    bool move_right()
    {
        if (position.x < 639)
        {
            obj.move(sf::Vector2f(XSPEED, 0)); 
            position.x += XSPEED;
            return true;
        }
        return false;
    }

    bool move_left()
    {
        if (position.x > 1)
        {
            obj.move(sf::Vector2f(-XSPEED, 0));  
            position.x -= XSPEED;
            return true;
        } 
        return false;
    }

    bool move_top()
    {
        if(position.y > 1)
        {
            obj.move(sf::Vector2f(0, -YSPEED));
            position.y -= YSPEED;
            return true;
        }
        return false;
    }

    bool move_bottom()
    {
        if(position.y < 479)
        {
            obj.move(sf::Vector2f(0, YSPEED));
            position.y += YSPEED;
            return true;
        }
        return false;
    }

    void reset()
    {
        obj.setPosition(0,0);
        position = obj.getPosition();
        obj.setRotation(0);
    }

    void rotate(int angle){
        obj.setRotation(angle);
    }

};


int main()
{
    // Creating the window and intial variables
    sf::RenderWindow window(sf::VideoMode(640, 480), "HW1 Part 2");
    
    sf::Event event;

    // Sprite initialization
    Sprite sprite1, sprite2, sprite3, sprite4;

    // Initialize status variables
    int count = 1;
    bool flag = true;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            // Window closes
            if (event.type == sf::Event::Closed){
                std::cout<<"Closing Window";
                window.close();
            }     
        }

        // Reset sprite position and orientation after a complete cycle
        if(count == 1 && flag)
        {   
            sprite1.reset();
            sprite2.reset();
            sprite3.reset();
            sprite4.reset();
            flag = false;
        }

        window.clear(sf::Color::White);

        switch(count)
        {

            case 1:
            if(sprite1.move_right())
            {
                window.draw(sprite1.obj);
                
                window.display();
            }
            else{
                count++;
                sprite1.rotate(90);
            }
            break;

            case 2:
            if(sprite2.move_right() && sprite1.move_bottom())
            {
                window.draw(sprite2.obj);
                window.draw(sprite1.obj);

                window.display();
            }
            else
            {
                count++;
                sprite1.rotate(180);
                sprite2.rotate(90);
            }
            break;

            case 3:
            if(sprite3.move_right() && sprite2.move_bottom() && sprite1.move_left())
            {
                window.draw(sprite3.obj);
                window.draw(sprite2.obj);
                window.draw(sprite1.obj);

                window.display();
            }
            else
            {
                count++;
                sprite1.rotate(270);
                sprite2.rotate(180);
                sprite3.rotate(90);
            }
            break;

            case 4:
            if(sprite4.move_right() && sprite3.move_bottom() && sprite2.move_left() && sprite1.move_top() )
            {
                window.draw(sprite4.obj);
                window.draw(sprite3.obj);
                window.draw(sprite2.obj);
                window.draw(sprite1.obj);

                window.display();
            }
            else
            {
                count++;
                sprite2.rotate(270);
                sprite3.rotate(180);
                sprite4.rotate(90);
            }
            break;

            case 5:
            if(sprite4.move_bottom() && sprite3.move_left() && sprite2.move_top())
            {
                window.draw(sprite4.obj);
                window.draw(sprite3.obj);
                window.draw(sprite2.obj);
                
                window.display();
            }
            else
            {
                count++;
                sprite3.rotate(270);
                sprite4.rotate(180);
            }
            break;

            case 6:
            if(sprite4.move_left() && sprite3.move_top())
            {
                window.draw(sprite4.obj);
                window.draw(sprite3.obj);
                
                window.display();
            }
            else
            {
                count++;
                sprite4.rotate(270);
            }
            break;

            case 7:
            if(sprite4.move_top())
            {
                window.draw(sprite4.obj);

                window.display();
            }
            else
            {
                count = 1;
                flag = true;
            }
            break;
        }
    }
}


