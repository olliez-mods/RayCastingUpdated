#include "ColorPicker.hpp"
using namespace sfas;

ColorPicker::ColorPicker(sf::Vector2f pos, sf::Vector2f size, float buffer)
{
    this->x = pos.x;
    this->y = pos.y;
    this->width = size.x;
    this->height = size.y;
    this->buffer = buffer;
    rect = sf::RectangleShape(sf::Vector2f(width, height));
    
    colorRect = sf::RectangleShape(sf::Vector2f(width - buffer*2, height/6*3 - buffer*2));
    colorRect.setFillColor(sf::Color::Black);
    
    sliderR = Slider(sf::Vector2f(x + buffer, y + height/6*3), sf::Vector2f(width - buffer*2, height/6-buffer), 0, 255);
    sliderG = Slider(sf::Vector2f(x + buffer, y + height/6*4), sf::Vector2f(width - buffer*2, height/6-buffer), 0, 255);
    sliderB = Slider(sf::Vector2f(x + buffer, y + height/6*5), sf::Vector2f(width - buffer*2, height/6-buffer), 0, 255);
    
    sliderR.doShowValue(true);
    sliderG.doShowValue(true);
    sliderB.doShowValue(true);
    
    sliderR.setColor(sf::Color::White, sf::Color(40, 40, 40));
    sliderG.setColor(sf::Color::White, sf::Color(40, 40, 40));
    sliderB.setColor(sf::Color::White, sf::Color(40, 40, 40));
    
    setPosition(pos);
}

ColorPicker::ColorPicker(){}

void ColorPicker::tick(sf::Window& window)
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
        
        clickedOnTick = (isHeld && !mouseDownLastTick);
        
        
        sliderR.tick(window);
        sliderG.tick(window);
        sliderB.tick(window);
        
        colorRect.setFillColor(sf::Color(sliderR.getValue(), sliderG.getValue(), sliderB.getValue()));
        
        
        mouseDownLastTick = mouseDown;
    }
}

sf::Color ColorPicker::getColor()
{
    return colorRect.getFillColor();
}

void ColorPicker::overrideColor(sf::Color color)
{
    sliderR.setValue(color.r);
    sliderG.setValue(color.g);
    sliderB.setValue(color.b);
}



void ColorPicker::setColor(sf::Color color)
{
    rect.setFillColor(color);
}

void ColorPicker::setTexture(std::string defaultTextureStr)
{
    
    texture.loadFromFile(defaultTextureStr);
    usingTextures = true;
    rect.setTexture(&texture);
}

void ColorPicker::setTexture(sf::Texture defaultTexture)
{
    usingTextures = true;
    texture = defaultTexture;
    rect.setTexture(&texture);
}

void ColorPicker::setPosition(sf::Vector2f pos)
{
    x = pos.x;
    y = pos.y;
    rect.setPosition(sf::Vector2f(x, y));
    colorRect.setPosition(sf::Vector2f(pos.x+buffer, pos.y+buffer));
    
    sliderR.setPosition(sf::Vector2f(x + buffer, y + height/6*3));
    sliderG.setPosition(sf::Vector2f(x + buffer, y + height/6*4));
    sliderB.setPosition(sf::Vector2f(x + buffer, y + height/6*5));
}

void ColorPicker::display(sf::RenderWindow& window)
{
    if(render)
    {
        window.draw(rect);
        window.draw(colorRect);
        sliderR.display(window);
        sliderG.display(window);
        sliderB.display(window);
    }
}

void ColorPicker::checkEvent(sf::Event event)
{
    
}


