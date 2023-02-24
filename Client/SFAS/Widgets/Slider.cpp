#include "Slider.hpp"
using namespace sfas;

Slider::Slider(sf::Vector2f pos, sf::Vector2f size, int min, int max)
{
    this->min = min;
    this->max = max;
    
    this->x = pos.x;
    this->y = pos.y;
    this->width = size.x;
    this->height = size.y;
    
    baseRect = sf::RectangleShape(sf::Vector2f(width, height));
    sliderRect = sf::RectangleShape(sf::Vector2f(5, height));
    baseRect.setFillColor(sf::Color::White);
    sliderRect.setFillColor(sf::Color::Blue);
    setPosition(pos);
    setValue(min);
}

Slider::Slider() {
    min = 0;
    max = 1;
}

void Slider::doShowValue(bool show, sf::Color color, int size)
{
    showValue = show;
    valueString.setFont(Widget::font);
    valueString.setFillColor(color);
    valueString.setCharacterSize(size);
}

void Slider::setPosition(sf::Vector2f pos)
{
    this->x = pos.x;
    this->y = pos.y;
    sliderRect.setPosition(sf::Vector2f(x + (sliderRect.getPosition().x - baseRect.getPosition().x), y));
    baseRect.setPosition(sf::Vector2f(x, y));
    valueString.setPosition(sf::Vector2f(baseRect.getPosition().x + baseRect.getSize().x / 2 - valueString.getLocalBounds().width / 2,
                            baseRect.getPosition().y + baseRect.getSize().y / 2 - valueString.getLocalBounds().height / 2));
}

void Slider::setColor(sf::Color baseColor, sf::Color sliderColor)
{
    baseRect.setFillColor(baseColor);
    sliderRect.setFillColor(sliderColor);
}

void Slider::setTexture(std::string baseTextureStr, std::string sliderTextureStr)
{
    baseTexture.loadFromFile(baseTextureStr);
    sliderTexture.loadFromFile(sliderTextureStr);
    baseRect.setTexture(&baseTexture);
    sliderRect.setTexture(&sliderTexture);
}

void Slider::setTexture(sf::Texture baseTexture, sf::Texture sliderTexture)
{
    this->baseTexture = baseTexture;
    this->sliderTexture = sliderTexture;
    
    baseRect.setTexture(&baseTexture);
    sliderRect.setTexture(&sliderTexture);
}

void Slider::tick(sf::Window& window)
{
    if(enabled)
    {
        sf::Mouse mouse;
        sf::Vector2i mousePos = mouse.getPosition(window);
        
        mouseOver = (mousePos.x > x && mousePos.x < x + baseRect.getSize().x &&
                     mousePos.y > y && mousePos.y < y + baseRect.getSize().y);
        
        bool mouseDown = mouse.isButtonPressed(mouse.Left);
        
        if(!mouseDownLastTick && mouseDown && mouseOver)
            isHeld = true;
        if(!mouseDown)
            isHeld = false;
        
        if (isHeld)
        {
            sliderRect.setPosition(sf::Vector2f(std::max(x, std::min((float)mousePos.x, x+width)), sliderRect.getPosition().y));
            
            float perc = ((baseRect.getSize().x - (sliderRect.getPosition().x - baseRect.getPosition().x)) / baseRect.getSize().x);
            value = perc * (min - max) + max;
        }
        
        std::string newValueString = (showValueAsInt) ? std::to_string((int)value) : std::to_string(value);
        valueString.setString(newValueString);
        valueString.setPosition(sf::Vector2f(baseRect.getPosition().x + baseRect.getSize().x / 2 - valueString.getLocalBounds().width / 2,
                                baseRect.getPosition().y + baseRect.getSize().y / 2 - valueString.getLocalBounds().height / 2));
        
        mouseDownLastTick = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    }
}

float Slider::getValue()
{
    return value;
}

void Slider::setValue(float newValue)
{
    value = newValue;
    sliderRect.setPosition(sf::Vector2f(baseRect.getPosition().x + ((newValue-min)/(max-min)*baseRect.getSize().x), sliderRect.getPosition().y));
}

void Slider::display(sf::RenderWindow& window)
{
    if(render)
    {
        window.draw(baseRect);
        if (showValue)
        {
            // valueString.setFont(font);
            window.draw(valueString);
        }
        window.draw(sliderRect);
    }
}

void Slider::checkEvent(sf::Event event)
{
    
}
