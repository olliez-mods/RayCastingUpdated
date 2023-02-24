#include "Button.hpp"
using namespace sfas;

Button::Button(sf::Vector2f pos, sf::Vector2f size)
{
    this-> x = pos.x;
    this->y = pos.y;
    this->width = size.x;
    this->height = size.y;
    rect = sf::RectangleShape(sf::Vector2f(width, height));
    setPosition(pos);
}

Button::Button(){}

void Button::tick(sf::Window& window)
{
    if(enabled)
    {
    sf::Mouse mouse;
    sf::Vector2i mousePos = mouse.getPosition(window);
    
    mouseOver = (mousePos.x > x && mousePos.x < x + rect.getSize().x &&
                 mousePos.y > y && mousePos.y < y + rect.getSize().y);
    
    bool mouseDown = mouse.isButtonPressed(mouse.Left);
    
        if(!mouseDownLastTick && mouseDown && mouseOver)
            isHeld = true;
        if(!mouseDown)
            isHeld = false;
    
    if (usingtextures)
    {
        if (isHeld)
            rect.setTexture(&pressedTexture);
        else if (mouseDownLastTick && !mouseDown)
            rect.setTexture(&buttonTexture);
    }
    
    clickedOnTick = (isHeld && !mouseDownLastTick);
    
    mouseDownLastTick = mouseDown;
    }
}

void Button::setColor(sf::Color color)
{
    rect.setFillColor(color);
}

void Button::setTexture(std::string defaultTextureStr, std::string pressedTextureStr)
{
    if (pressedTextureStr == "-1a")
        pressedTextureStr = defaultTextureStr;
    
    buttonTexture.loadFromFile(defaultTextureStr);
    pressedTexture.loadFromFile(pressedTextureStr);
    usingtextures = true;
    rect.setTexture(&buttonTexture);
}

void Button::setTexture(sf::Texture defaultTexture)
{
    usingtextures = true;
    buttonTexture = defaultTexture;
    this->pressedTexture = defaultTexture;
    rect.setTexture(&buttonTexture);
}

void Button::setTexture(sf::Texture defaultTexture, sf::Texture pressedTexture)
{
    usingtextures = true;
    buttonTexture = defaultTexture;
    this->pressedTexture = pressedTexture;
    rect.setTexture(&buttonTexture);
}

void Button::setText(std::string text, sf::Color color, int textSize)
{
    rectString.setFont(font);
    rectString.setString(text);
    rectString.setCharacterSize(textSize);
    rectString.setFillColor(color);
    rectString.setPosition(sf::Vector2f(x + width / 2 - rectString.getLocalBounds().width / 2,
                           y + height / 2 - rectString.getCharacterSize() / 1.5));
}

std::string Button::getText()
{
    return rectString.getString();
}

void Button::setPosition(sf::Vector2f pos)
{
    x = pos.x;
    y = pos.y;
    rect.setPosition(sf::Vector2f(x, y));
    rectString.setPosition(sf::Vector2f(rect.getPosition().x + rect.getSize().x / 2-rectString.getLocalBounds().width / 2,
                           rect.getPosition().y + rect.getSize().y / 2 - rectString.getLocalBounds().height));
}

void Button::display(sf::RenderWindow& window)
{
    if(render)
    {
    window.draw(rect);
    window.draw(rectString);
    }
}

void Button::checkEvent(sf::Event event)
{
    
}
