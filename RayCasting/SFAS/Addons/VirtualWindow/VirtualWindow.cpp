#include "VirtualWindow.hpp"
using namespace sfas;

VirtualWindow::VirtualWindow(sf::Vector2f pos, sf::Vector2f size, Widget* widget)
{
    this->x = pos.x;
    this->y = pos.y;
    this->width = size.x;
    this->height = size.y;
    this->widget = widget;
    
    rect = sf::RectangleShape(sf::Vector2f(width, height));
    
    float b = height/10;
    hideSwitch = Switch(sf::Vector2f(x + width - height + b, y + b), sf::Vector2f(height-b*2, height-b*2));
    hideSwitch.setColor(sf::Color(200, 200, 200), sf::Color(120, 120, 120));
    
    
    setPosition(pos);
}

VirtualWindow::VirtualWindow(){}

void VirtualWindow::tick(sf::Window& window)
{
    if(enabled)
    {
        hideSwitch.tick(window);
        
        sf::Mouse mouse;
        sf::Vector2i mousePos = mouse.getPosition(window);
        
        mouseOver = (mousePos.x > x && mousePos.x < x + width &&
                     mousePos.y > y && mousePos.y < y + height);
        
        bool mouseDown = mouse.isButtonPressed(mouse.Left);
        clickedOnTick = (mouseOver && !mouseDownLastTick && mouseDown);
        
        if(!mouseDownLastTick && mouse.isButtonPressed(mouse.Left) && mouseOver)
            isHeld = true;
        if(!mouse.isButtonPressed(mouse.Left))
            isHeld = false;

        hidden = hideSwitch.getValue();
        
        if(clickedOnTick)
        {
            offset.x = x - mousePos.x;
            offset.y = y - mousePos.y;
        }
        if(isHeld)
            setPosition(sf::Vector2f(std::max(0.0f, std::min((float)mousePos.x + offset.x, (float)window.getSize().x - width)),
                                     std::max(0.0f, std::min((float)mousePos.y + offset.y, (float)window.getSize().y - height))));

        if(hideSwitch.clickedOnTick){
            widget->enabled = !hideSwitch.getValue();
            widget->render = !hideSwitch.getValue();
        }
        
        mouseDownLastTick = mouseDown;
    }
}

void VirtualWindow::setText(std::string text, int size)
{
    stringText.setFont(font);
    stringText.setString(text);
    stringText.setCharacterSize(size);
    stringText.setFillColor(sf::Color::Black);
    stringText.setPosition(sf::Vector2f(x, y + height / 2 - stringText.getCharacterSize() / 2));
}

void VirtualWindow::setColor(sf::Color color)
{
    rect.setFillColor(color);
}

void VirtualWindow::setTexture(std::string textureStr)
{
    
    texture.loadFromFile(textureStr);
    rect.setTexture(&texture);
}

void VirtualWindow::setTexture(sf::Texture texture)
{
    this->texture = texture;
    rect.setTexture(&this->texture);
}

void VirtualWindow::setPosition(sf::Vector2f pos)
{
    x = pos.x;
    y = pos.y;
    rect.setPosition(sf::Vector2f(x, y));
    widget->setPosition(sf::Vector2f(pos.x, pos.y+height));
    hideSwitch.setPosition(sf::Vector2f(x + width - height + height/10, y + height/10));
    stringText.setPosition(sf::Vector2f(x, y + height / 2 - stringText.getCharacterSize() / 2));
}

void VirtualWindow::display(sf::RenderWindow& window)
{
    if(render)
    {
        window.draw(rect);
        window.draw(stringText);
        hideSwitch.display(window);
    }
}

void VirtualWindow::checkEvent(sf::Event event)
{
    
}


