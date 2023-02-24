#include "Input.hpp"
using namespace sfas;

Input::Input(sf::Vector2f pos, sf::Vector2f size, sf::Color color)
{
    this->x = pos.x;
    this->y = pos.y;
    this->width = size.x;
    this->height = size.y;
    
    stringText.setFillColor(sf::Color::Black);
    stringText.setFont(font);
    stringText.setCharacterSize(height);
    
    rect = sf::RectangleShape(sf::Vector2f(width, height));
    rect.setFillColor(color);
    setPosition(pos);
}

Input::Input() {}

void Input::tick(sf::Window& window)
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
        
        if (mouseDown && !mouseOver)
            inFocus = false;
        
        if (inFocus) {
            while (keysPressedThisTick.size() > 0)
            {
                int key = keysPressedThisTick[0];
                char keyChar = static_cast<char>(key);
                if (key == 8) {
                    value = value.substr(0, value.size() - 1);
                    setText(value);
                }
                else {
                    value += keyChar;
                    setText(value);
                }
                keysPressedThisTick.erase(keysPressedThisTick.begin(), keysPressedThisTick.begin() + 1);
            }
        }
        
        
        mouseDownLastTick = mouse.isButtonPressed(mouse.Left);
    }
}

void Input::setTextSize(int textSize)
{
    stringText.setCharacterSize(textSize);
}

std::string Input::getText()
{
    return value;
}

void Input::setText(std::string text)
{
    value = text;
    stringText.setString(text);
    stringText.setPosition(sf::Vector2f(x, y + height / 2 - stringText.getCharacterSize() / 1.5));
}

void Input::setTexture(sf::Texture texture)
{
    rectTexture = texture;
    rect.setTexture(&rectTexture);
}
void Input::setTexture(std::string textureStr)
{
    rectTexture.loadFromFile(textureStr);
    rect.setTexture(&rectTexture);
}

void Input::setTextColor(sf::Color color)
{
    stringText.setFillColor(color);
}

void Input::setColor(sf::Color color)
{
    rect.setFillColor(color);
}

void Input::setPosition(sf::Vector2f pos)
{
    this->x = pos.x;
    this->y = pos.y;
    rect.setPosition(sf::Vector2f(x, y));
    setText(stringText.getString());
}

void Input::display(sf::RenderWindow& window)
{
    if(render)
    {
        window.draw(rect);
        window.draw(stringText);
    }
}

void Input::checkEvent(sf::Event event)
{
    keysPressedThisTick.clear();
    if (event.type == sf::Event::TextEntered)
    {
        if (event.text.unicode < 128)
            keysPressedThisTick.push_back(event.text.unicode);
    }
}
