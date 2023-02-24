#ifndef VirtualWindow_hpp
#define VirtualWindow_hpp

#include "../../Widget.hpp"
#include "../../Widgets/Switch.hpp"

namespace sfas
{
class VirtualWindow : public Widget
{
    bool mouseDownLastTick = true;
    
    bool hidden = false;
    Switch hideSwitch;
    
    sf::Vector2f offset;
    
    sf::Text stringText;
    sf::Texture texture;
public:
    
    Widget* widget;
    
    bool clickedOnTick = false;
    sf::RectangleShape rect;
    void setPosition(sf::Vector2f pos);
    void tick(sf::Window& window);
    void display(sf::RenderWindow& window);
    void checkEvent(sf::Event event);
    void setColor(sf::Color color);
    
    void setText(std::string text, int size);
    
    void setTexture(std::string defaultTexture);
    void setTexture(sf::Texture defaultTexture);

    VirtualWindow(sf::Vector2f pos, sf::Vector2f size, Widget* widget);
    VirtualWindow();
};
}

#endif
