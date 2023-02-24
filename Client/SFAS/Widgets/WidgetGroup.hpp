#ifndef WidgetGroup_hpp
#define WidgetGroup_hpp
#include "../Widget.hpp"

namespace sfas
{
class WidgetGroup : public Widget
{
    bool enabledCheck = true;
    bool renderCheck = true;
    
    std::vector<Widget*> widgetStack;
public:
    void setPosition(sf::Vector2f pos);
    void addPosition(sf::Vector2f pos);
    
    void addWidget(Widget* widget);
    
    void tick(sf::Window& window);
    void display(sf::RenderWindow& window);
    void checkEvent(sf::Event event);
    
    WidgetGroup(sf::Vector2f pos);
    WidgetGroup();
};
}
#endif
