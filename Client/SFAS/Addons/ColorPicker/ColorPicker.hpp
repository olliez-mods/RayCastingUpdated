#ifndef ColorPicker_hpp
#define ColorPicker_hpp

#include "../../Widget.hpp"
#include "../../Widgets/Slider.hpp"

namespace sfas
{
class ColorPicker : public Widget
{
    bool usingTextures = false;
    bool mouseDownLastTick = true;

    float buffer;
    
    sfas::Slider sliderR;
    sfas::Slider sliderG;
    sfas::Slider sliderB;
    
    sf::RectangleShape colorRect;
    
    sf::Texture texture;
public:
   // bool enabled = true;
   // bool render = true;
    
    bool clickedOnTick = false;
    sf::RectangleShape rect;
    void setPosition(sf::Vector2f pos);
    void tick(sf::Window& window);
    void display(sf::RenderWindow& window);
    void checkEvent(sf::Event event);
    void setColor(sf::Color color);

    sf::Color getColor();
    void overrideColor(sf::Color color);
    
    void setTexture(std::string defaultTexture);
    void setTexture(sf::Texture defaultTexture);

    ColorPicker(sf::Vector2f pos, sf::Vector2f size, float buffer);
    ColorPicker();
};
}

#endif
