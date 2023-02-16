#ifndef Switch_hpp
#define Switch_hpp
#include "../Widget.hpp"

namespace sfas
{
class Switch : public Widget
{
    bool mouseDownLastTick = true;
    bool value = false;

    sf::Color defaultColor = sf::Color::Red;
    sf::Color activatedColor = sf::Color::Green;
    
    sf::Texture offTexture;
    sf::Texture onTexture;

    bool usingTextures = false;
public:
    bool clickedOnTick = false;
    sf::RectangleShape baseRect;
    void setValue(bool newValue);
    bool getValue();
    void setPosition(sf::Vector2f pos);
    void setTexture(std::string offTexture, std::string onTexture);
    void setTexture(sf::Texture offTexture, sf::Texture onTexture);
    void setColor(sf::Color defaultColor, sf::Color activatedColor);
    void tick(sf::Window& window);
    void display(sf::RenderWindow& window);
    void checkEvent(sf::Event event);
    Switch(sf::Vector2f pos, sf::Vector2f size);
    Switch();
};
}
#endif
