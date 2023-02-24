#include "Switch.hpp"
using namespace sfas;

Switch::Switch(sf::Vector2f pos, sf::Vector2f size)
{
    
    this->x = pos.x;
    this->y = pos.y;
    this->width = size.x;
    this->height = size.y;
    
    baseRect = sf::RectangleShape(sf::Vector2f(width, height));
    baseRect.setFillColor(sf::Color::Red);
    
    setPosition(pos);
    setValue(false);
}

Switch::Switch() {}

bool Switch::getValue()
{
    return value;
}

void Switch::setValue(bool newValue)
{
    value = newValue;
    if (usingTextures) {
        baseRect.setTexture(&offTexture);
        if (newValue)
            baseRect.setTexture(&onTexture);
        return;
    }
    
    baseRect.setFillColor(defaultColor);
    if (newValue)
        baseRect.setFillColor(activatedColor);
    
}

void Switch::setTexture(std::string offTextureStr, std::string onTextureStr)
{
    offTexture.loadFromFile(offTextureStr);
    onTexture.loadFromFile(onTextureStr);
    
    baseRect.setFillColor(sf::Color::White);
    usingTextures = true;
    setValue(value);
}
void Switch::setTexture(sf::Texture offTexture, sf::Texture onTexture)
{
    this->offTexture = offTexture;
    this->onTexture = onTexture;
    
    baseRect.setTexture(&offTexture);
    baseRect.setFillColor(sf::Color::White);
    usingTextures = true;
}

void Switch::setColor(sf::Color defaultColor, sf::Color activatedColor)
{
    this->defaultColor = defaultColor;
    this->activatedColor = activatedColor;
}

void Switch::tick(sf::Window& window)
{
    if(enabled)
    {
        sf::Mouse mouse;
        sf::Vector2i mousePos = mouse.getPosition(window);
        
        mouseOver = (mousePos.x > x && mousePos.x < x + baseRect.getSize().x &&
                     mousePos.y > y && mousePos.y < y + baseRect.getSize().y);
        
        //isHeld = (mouseOver && mouse.isButtonPressed(mouse.Left));
        bool mouseDown = mouse.isButtonPressed(mouse.Left);
        
        if(!mouseDownLastTick && mouseDown && mouseOver)
            isHeld = true;
        if(!mouseDown)
            isHeld = false;
        
        clickedOnTick = (isHeld && !mouseDownLastTick);
        
        if (clickedOnTick)
            setValue(!value);
        
        mouseDownLastTick = mouse.isButtonPressed(mouse.Left);
    }
}


void Switch::setPosition(sf::Vector2f pos)
{
    this->x = pos.x;
    this->y = pos.y;
    baseRect.setPosition(sf::Vector2f(x, y));
    setValue(value);
}

void Switch::display(sf::RenderWindow& window)
{
    if(render)
    {
        window.draw(baseRect);
    }
}

void Switch::checkEvent(sf::Event event)
{
    
}
