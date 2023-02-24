#ifndef Button_hpp
#define Button_hpp
#include "../Widget.hpp"

namespace sfas
{
class Button : public Widget
{
    bool usingtextures = false;
    bool mouseDownLastTick = true;
    sf::Text rectString;

    sf::Texture buttonTexture;
    sf::Texture pressedTexture;
public:
   // bool enabled = true;
   // bool render = true;
    
    bool clickedOnTick = false;
    sf::RectangleShape rect;
    void setPosition(sf::Vector2f pos);
    void tick(sf::Window& window);
    void setText(std::string text, sf::Color color, int textSize);
    std::string getText();
    void display(sf::RenderWindow& window);
    void checkEvent(sf::Event event);
    void setColor(sf::Color color);

    void setTexture(std::string defaultTexture, std::string pressedTexture = "-1a");
    void setTexture(sf::Texture defaultTexture);
    void setTexture(sf::Texture defaultTexture, sf::Texture pressedTexture);

    Button(sf::Vector2f pos, sf::Vector2f size);
    Button();
};
}
#endif
