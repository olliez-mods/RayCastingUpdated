#include "WidgetGroup.hpp"
using namespace sfas;

WidgetGroup::WidgetGroup(sf::Vector2f pos)
{
    this->x = pos.x;
    this->y = pos.y;
    //setPosition(pos);
}

WidgetGroup::WidgetGroup(){}

void WidgetGroup::addWidget(Widget *widget)
{
    widgetStack.push_back(widget);
}


void WidgetGroup::tick(sf::Window& window)
{
    if(enabled != enabledCheck){
        enabledCheck = enabled;
        for (Widget* w : widgetStack)
        {
            w->enabled = enabled;
        }
    }
    
    if(render != renderCheck){
        renderCheck = render;
        for (Widget* w : widgetStack)
        {
            w->render = render;
        }
    }
}

void WidgetGroup::display(sf::RenderWindow& window){}
void WidgetGroup::checkEvent(sf::Event event){}


void WidgetGroup::setPosition(sf::Vector2f pos)
{
    for (Widget* w : widgetStack)
    {
        w->setPosition(sf::Vector2f(pos.x+w->x-x, pos.y+w->y-y));
    }
    x = pos.x;
    y = pos.y;
}

void WidgetGroup::addPosition(sf::Vector2f pos)
{
    for (Widget* w : widgetStack)
    {
        w->setPosition(sf::Vector2f(pos.x+w->x, pos.y+w->y));
    }
    x += pos.x;
    y += pos.y;
}
