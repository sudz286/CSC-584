#include<string>
#include<iostream>
#include<SFML/Graphics.hpp>
#include<vector>
#include<random>
#include<chrono>

using namespace std;


class Boid
{
    public:
        sf::CircleShape boid;
        float radius;

        sf::Vector2f velocity;
        sf::Vector2f force;

        Boid(const sf::Vector2f& init_pos, const sf::Vector2f init_vel)
        :velocity(init_vel)
        {
            radius = 5;
            boid.setPosition(init_pos);
            boid.setRadius(radius);
            boid.setOrigin(radius, radius);
            boid.setFillColor(sf::Color(255, 0, 0));
        }

        void AddForce(const sf::Vector2f& force)
        {
            this->force += force;
            velocity += force;
        }

        void Move()
        {   
            boid.move(velocity);
        }

};


class Flock
{
    public:
        float alignMul, sepMul, cohMul;

        Flock(sf::RenderWindow* win)
        :window(win)
        {
            minimumSeperation = 40;
            maxForce = 0.01;
            maxVelocity = 0.1;

            // Adjust coefficients of alignment, cohesion and seperation
            alignMul = 2;
            cohMul = 1.1;
            sepMul = 1;

            m_seed = std::chrono::steady_clock::now().time_since_epoch().count();
            m_randomEngine.seed(m_seed);
            std::normal_distribution<float> igaussian(640 / 2, 0.2);
            std::uniform_real_distribution<float> uniformDist(-1, 1);


            uniform = uniformDist;
            gaussian = igaussian;


            for (int i = 0; i < 30; i++)
                add();

        }

        void update()
        {
            findAvgVal();
            Move();
            Display();
        }

    private:
    
        std::vector<Boid> boids;
        sf::RenderWindow* window = nullptr;

        std::uniform_real_distribution<float> uniform;
        int m_seed;
        std::mt19937 m_randomEngine;
        std::normal_distribution<float> gaussian;

        float maxForce;
        float maxVelocity;
        float minimumSeperation;

        void Move()
        {
            for (Boid& boid : boids)
            {
                boid.Move();
                checkBorder(boid);
            }
        }


        float absDist(const Boid& boid1, const Boid& boid2)
        {
            sf::Vector2f b1 = boid1.boid.getPosition();
            sf::Vector2f b2 = boid2.boid.getPosition();
            return sqrt(pow((b2.x - b1.x), 2) + pow((b2.y - b1.y), 2));
        }

        sf::Vector2f vecDist(const Boid& boid1, const Boid& boid2)
        {
            sf::Vector2f b1 = boid1.boid.getPosition();
            sf::Vector2f b2 = boid2.boid.getPosition();
            return b2 - b1;
        }
        
        float setMagnitude(const sf::Vector2f& vec)
        {
            return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
        }

        void restrictMagnitude(sf::Vector2f& vec,const float& limit)
        {
            float magnitude = setMagnitude(vec);
            if (magnitude > limit)
            {
                setMagnitude(vec, limit);
            }
        }

        void setMagnitude(sf::Vector2f& vec, const float& newmagnitude)
        {
            float currentMag = setMagnitude(vec);
            vec /= currentMag;
            vec *= newmagnitude;
        }

        bool ifEqual(const Boid& boid1, const Boid& boid2)
        {
            return &boid1 != &boid2;
        }

        void checkBorder(Boid& boid)
        {
            if (boid.boid.getPosition().x < -boid.radius) boid.boid.setPosition(sf::Vector2f{ 640 + boid.radius, boid.boid.getPosition().y });
            if (boid.boid.getPosition().x > 640 + boid.radius) boid.boid.setPosition(sf::Vector2f{ 0 - boid.radius, boid.boid.getPosition().y });
            if (boid.boid.getPosition().y < -boid.radius) boid.boid.setPosition(sf::Vector2f{ boid.boid.getPosition().x, 480 + boid.radius });
            if (boid.boid.getPosition().y > 480 + boid.radius) boid.boid.setPosition(sf::Vector2f{ boid.boid.getPosition().x, 0 - boid.radius });
        }

        void findAvgVal()
        {
            float alignmentperceptionradius = 20;
            float cohesionperceptionradius = 50;
            float seperationperceptionradius = 10;
            
            for (Boid& thisBoid : boids)
            {
                int alignCount = 0, cohCount = 0,sepCount = 0;

                sf::Vector2f AlignmentForce, CohesionForce = {}, SeperationForce = {}, dist = {};

                for (const Boid& otherBoid : boids)
                {
                    float d = absDist(thisBoid, otherBoid);

                    if (ifEqual(thisBoid, otherBoid) && d < alignmentperceptionradius)
                    {
                        AlignmentForce += otherBoid.velocity;
                        alignCount++;
                    }
                    if (ifEqual(thisBoid, otherBoid) && d < cohesionperceptionradius)
                    {
                        CohesionForce += otherBoid.boid.getPosition();
                        cohCount++;
                    }
                    if (ifEqual(thisBoid, otherBoid) && d < seperationperceptionradius)
                    {
                        dist = thisBoid.boid.getPosition() - otherBoid.boid.getPosition();
                        dist /= (d);
                        SeperationForce += dist;
                        sepCount++;
                    }

                }

                if (alignCount > 0)
                {
                    AlignmentForce /= float(alignCount);
                    setMagnitude(AlignmentForce, maxVelocity);
                    AlignmentForce = AlignmentForce - thisBoid.velocity;
                    restrictMagnitude(AlignmentForce, maxForce);
                }

                if (cohCount > 0)
                {
                    CohesionForce /= float(cohCount);
                    CohesionForce = CohesionForce - thisBoid.boid.getPosition();
                    setMagnitude(CohesionForce, maxVelocity);
                    CohesionForce = CohesionForce - thisBoid.velocity;
                    restrictMagnitude(CohesionForce, maxForce);
                }
                
                if (sepCount > 0)
                {
                    SeperationForce /= float(sepCount);
                    setMagnitude(SeperationForce, maxVelocity);
                    SeperationForce = SeperationForce - thisBoid.velocity;
                    restrictMagnitude(SeperationForce, maxForce);
                }

                AlignmentForce *= alignMul;
                thisBoid.AddForce(AlignmentForce);
                CohesionForce *= cohMul;
                thisBoid.AddForce(CohesionForce);
                SeperationForce *= sepMul;
                thisBoid.AddForce(SeperationForce);
            }

        }

        void Display()
        {
            for (const Boid& boid : boids) window->draw(boid.boid);
            window->display();
        }

        void add()
        {
            Boid b2(sf::Vector2f{ gaussian(m_randomEngine), gaussian(m_randomEngine) }, sf::Vector2f{ float(uniform(m_randomEngine)), float(uniform(m_randomEngine)) });
            boids.push_back(b2);
        }
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "Flocking", sf::Style::Default);

    Flock f(&window);
    while (window.isOpen())
    {
        sf::Event event;
        window.clear(sf::Color::White);
        while (window.pollEvent(event))
        {  
            if(event.type == sf::Event::EventType::Closed)
            {
                cout<<"Closing Window";
                window.close();
            }
        }

        f.update();
    }   
}
