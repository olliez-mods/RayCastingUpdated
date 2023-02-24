#include "../Widget.hpp"
using namespace sfas;

sf::Font Widget::font = sf::Font();
void Widget::display(sf::RenderWindow& window){}
void Widget::tick(sf::Window& window){}
void Widget::setPosition(sf::Vector2f pos){}
void Widget::checkEvent(sf::Event event) {}
void Widget::setFont(std::string path)
{
    font = sf::Font();
    font.loadFromFile(path);
}

void Widget::setFont(sf::Font nfont)
{
    font = nfont;
}

//==========================
//    WIDGET manager CLASS
//==========================

WidgetManager::WidgetManager()
{
    widgetStack = std::vector<Widget*>();
}

void WidgetManager::registerWidget(Widget* widget){
    widgetStack.push_back(widget);
}

void WidgetManager::clearStack() {
    widgetStack.clear();
}

void WidgetManager::tickStack(sf::RenderWindow& window) {
    for (Widget* widget : widgetStack)
    {
        widget->tick(window);
    }
}

void WidgetManager::renderStack(sf::RenderWindow& window) {
    for (Widget* widget : widgetStack)
    {
        widget->display(window);
    }
}

void WidgetManager::checkEventForStack(sf::Event event)
{
    for (Widget* widget : widgetStack)
    {
        widget->checkEvent(event);
    }
}
