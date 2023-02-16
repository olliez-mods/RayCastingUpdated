#ifndef Animation_hpp
#define Animation_hpp
#include "../Widget.hpp"
namespace sfas
{
class Animation : public Widget
{
    bool mouseDownLastTick = true;
    
    bool playing;
    int frame = 0;
    std::vector<sf::Texture> frames;
    
    sf::Clock clock;
    float aniTime = -1;
    float timer = 0;
public:
    bool clickedOnTick = false;
    bool loop = false;
    sf::RectangleShape baseRect;
    void play();
    void stop();
    void setFrames(std::vector<sf::Texture> nFrames, float time);
    void setPosition(sf::Vector2f pos);
    void tick(sf::Window& window);
    void display(sf::RenderWindow& window);
    void checkEvent(sf::Event event);
    Animation(sf::Vector2f pos, sf::Vector2f size);
    Animation();
};
}

#endif
