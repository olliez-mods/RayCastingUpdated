#include "Scroller.hpp"
using namespace sfas;

Scroller::Scroller(sf::Vector2f pos, sf::Vector2f size)
{
    this->x = pos.x;
    this->y = pos.y;
    this->width = size.x;
    this->height = size.y;
    
    rect = sf::RectangleShape(sf::Vector2f(width, height));
    rect.setFillColor(sf::Color(230, 230, 230));
    setPosition(pos);
}

Scroller::Scroller() {
}

void Scroller::tick(sf::Window& window)
{
    if(enabled)
    {
        sf::Mouse mouse;
        sf::Vector2i mousePos = mouse.getPosition(window);
        
        mouseOver = (mousePos.x > x && mousePos.x < x + rect.getSize().x &&
                     mousePos.y > y && mousePos.y < y + rect.getSize().y);
        
        clickedOnTick = (mouseOver && mouse.isButtonPressed(mouse.Left) && !mouseDownLastTick);
        
        bool mouseDown = mouse.isButtonPressed(mouse.Left);
        
        if(!mouseDownLastTick && mouseDown && mouseOver)
            isHeld = true;
        if(!mouseDown)
            isHeld = false;
        
        if (clickedOnTick)
            inFocus = true;
        
        if (mouse.isButtonPressed(mouse.Left) && !mouseOver)
            inFocus = false;
        
        if (inFocus || mouseOver) {
            if(scrollDist != 0)
            {
                bool allow = false;
                for(Widget* w : widgetVec)
                {
                    if(w->y < y + buffer && scrollDist > 0)
                        allow = true;
                    
                    if(w->y + w->height > y + height - buffer && scrollDist < 0)
                        allow = true;
                }
                if(allow)
                {
                    for(Widget* w : widgetVec)
                    {
                        w->setPosition(sf::Vector2f(w->x, w->y+scrollDist));
                        if((w->y + w->height > y+height || w->y < y) && disableUnfocusedWidgets)
                        {
                            w-> render = false;
                            w-> enabled = false;
                        }else
                        {
                            w-> render = true;
                            w-> enabled = true;
                        }
                        
                        
                    }
                    
                }
            }
            scrollDist = 0;
        }
        
        mouseDownLastTick = mouse.isButtonPressed(mouse.Left);
    }
}

void Scroller::setTexture(sf::Texture texture)
{
    rectTexture = texture;
    rect.setTexture(&rectTexture);
}
void Scroller::setTexture(std::string textureStr)
{
    rectTexture.loadFromFile(textureStr);
    rect.setTexture(&rectTexture);
}

void Scroller::setColor(sf::Color color)
{
    rect.setFillColor(color);
}

void Scroller::setPosition(sf::Vector2f pos)
{
    this->x = pos.x;
    this->y = pos.y;
    rect.setPosition(sf::Vector2f(x, y));
}

void Scroller::addWidget(Widget* widget)
{
    widgetVec.push_back(widget);
}

/*
 void setWidgetManager(WidgetManager &widgetMan)
 {
 
 }
 //*/

void Scroller::display(sf::RenderWindow& window)
{
    if(render)
    {
        window.draw(rect);
    }
}

void Scroller::checkEvent(sf::Event event)
{

    if (event.type == sf::Event::MouseWheelScrolled)
    {
        scrollDist = event.mouseWheelScroll.delta*scrollSpeed;
    }
    
    if(event.type == sf::Event::KeyPressed)
    {
        if(event.text.unicode == 73)
        {
            scrollDist = -30;
        }
        if(event.text.unicode == 74)
        {
            scrollDist = 30;
        }
    }
}
