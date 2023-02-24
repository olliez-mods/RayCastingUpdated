#ifndef Slider_hpp
#define Slider_hpp
#include "../Widget.hpp"

namespace sfas
{
class Slider : public Widget
{
    sf::Text valueString;
    float value = 0;
    bool showValue = false;

    bool isHeld = false;
    bool mouseDownLastTick = false;
    
    sf::Texture baseTexture;
    sf::Texture sliderTexture;
public:
    int min;
    int max;

    bool showValueAsInt = true;
    sf::RectangleShape baseRect;
    sf::RectangleShape sliderRect;
    void doShowValue(bool show, sf::Color color = sf::Color::Black, int size = 30);
    void setPosition(sf::Vector2f pos);
    void setColor(sf::Color baseColor, sf::Color sliderColor);
    void setTexture(std::string baseTexture, std::string sliderTexture);
    void setTexture(sf::Texture baseTexture, sf::Texture sliderTexture);
    void setValue(float newValue);
    float getValue();
    void tick(sf::Window& window);
    void display(sf::RenderWindow& window);
    void checkEvent(sf::Event event);
    Slider(sf::Vector2f pos, sf::Vector2f size, int min, int max);
    Slider();
};
}

#endif
