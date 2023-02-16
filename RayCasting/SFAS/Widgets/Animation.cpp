#include "Animation.hpp"
using namespace sfas;

Animation::Animation(sf::Vector2f pos, sf::Vector2f size)
{
    
    this-> x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    
    baseRect = sf::RectangleShape(sf::Vector2f(width, height));
    setPosition(pos);
}

Animation::Animation(){}

void Animation::setPosition(sf::Vector2f pos)
{
    this-> x = x;
    this-> y = y;
    baseRect.setPosition(sf::Vector2f(x, y));
}

void Animation::tick(sf::Window& window)
{
    if(enabled)
    {
    if(playing)
    {
        timer += clock.restart().asSeconds();
        if(timer >= aniTime/(frames.size()+1))
        {
            timer -= aniTime/(frames.size()+1);
            frame++;
            if(frame >= frames.size())
            {
                frame = 0;
                if(!loop)
                    stop();
            }
            if(playing){
                baseRect.setTexture(&frames[frame]);
                
            }
        }
    }else{
        clock.restart();
    }
    }
}

void Animation::play()
{
    frame = 0;
    timer = 0;
    playing = true;
}

void Animation::stop()
{
    playing = false;
    frame = 0;
}

void Animation::setFrames(std::vector<sf::Texture> nFrames, float time)
{
    aniTime = time;
    frames = nFrames;
    baseRect.setTexture(&frames[0]);
}

void Animation::display(sf::RenderWindow& window)
{
    if(render)
    window.draw(baseRect);
}
void Animation::checkEvent(sf::Event event)
{}
