#ifndef Scroller_hpp
#define Scroller_hpp

#include "../../Widget.hpp"

namespace sfas
{
class Scroller : public Widget
{
    bool mouseDownLastTick = true;
    float scrollDist = 0;
    
    std::vector<Widget*> widgetVec;
    WidgetManager *wMan;
    
    sf::Texture rectTexture;
public:
    bool inFocus = false;
    bool clickedOnTick = false;
    bool disableUnfocusedWidgets = true;
    float buffer = 0;
    sf::RectangleShape rect;
    float scrollSpeed = 100;
    void setPosition(sf::Vector2f pos);
    void tick(sf::Window& window);
    void setTexture(sf::Texture texture);
    void setTexture(std::string texture);
    void setColor(sf::Color color);
    void display(sf::RenderWindow& window);
    void checkEvent(sf::Event event);
    void addWidget(Widget* widget);
    //void setWidgetManager(WidgetManager &widgetMan);
    Scroller(sf::Vector2f pos, sf::Vector2f size);
    Scroller();
};
}

#endif
