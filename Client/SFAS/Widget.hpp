#ifndef Widget_hpp
#define Widget_hpp

#include <string>
#include <cmath>
#include <vector>
#include "SFML/Graphics.hpp"

namespace sfas
{

class Widget
{
public:
    static sf::Font font;
    
    float x, y;
    float width, height;
    
    bool enabled = true;
    bool render = true;
    
    static void setFont(std::string path);
    static void setFont(sf::Font font);

    bool mouseOver = false;
    bool isHeld = false;
    virtual void display(sf::RenderWindow& window);
    virtual void tick(sf::Window& window);
    virtual void setPosition(sf::Vector2f pos);
    virtual void checkEvent(sf::Event event);
};

class WidgetManager
{
    std::vector<Widget*> widgetStack;
public:
    WidgetManager();
    void registerWidget(Widget* widget);
    void clearStack();
    void tickStack(sf::RenderWindow& window);
    void renderStack(sf::RenderWindow& window);
    void checkEventForStack(sf::Event event);
};
}
#endif
