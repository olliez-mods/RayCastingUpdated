#include "Canvas.hpp"
using namespace sfas;

bool Canvas::validSpot(int x, int y)
{
    if(x >= 0 && x < img.getSize().x && y >= 0 && y < img.getSize().y)
        return true;
    return false;
}
bool Canvas::validSpot(sf::Vector2i pos)
{
    if(pos.x >= 0 && pos.x < img.getSize().x && pos.y >= 0 && pos.y < img.getSize().y)
        return true;
    return false;
}

Canvas::Canvas(sf::Vector2f pos, sf::Vector2f size)
{
    
    this->x = pos.x;
    this->y = pos.y;
    this->width = size.x;
    this->height = size.y;
    
    rect = sf::RectangleShape(sf::Vector2f(width, height));
    setPosition(pos);
}
Canvas::Canvas(){}

void Canvas::setPosition(sf::Vector2f pos)
{
    this->x = pos.x;
    this->y = pos.y;
    rect.setPosition(sf::Vector2f(x, y));
}
void Canvas::tick(sf::Window& window)
{
    if(enabled)
    {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        mouseOver = (pos.x >= x && pos.x < x + rect.getSize().x && pos.y >= y && pos.y < y + rect.getSize().y);
        bool mouseDown = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        
        if(!mouseDownLastTick && mouseDown && mouseOver)
            isHeld = true;
        if(!mouseDown)
            isHeld = false;
        
        mouseDownLastTick = mouseDown;
    }
}
void Canvas::display(sf::RenderWindow& window)
{
    if(render)
    {
        texture.loadFromImage(img);
        window.draw(rect);
    }
}
void Canvas::checkEvent(sf::Event event){}

void Canvas::create(sf::Vector2f size)
{
    img.create(sf::Vector2u(size.x, size.y));
    texture.create(sf::Vector2u(size.x, size.y));
    rect.setTexture(&texture);
}



sf::Vector2i Canvas::getSize()
{
    return sf::Vector2i(img.getSize().x, img.getSize().y);
}
sf::Vector2f Canvas::getBounds()
{
    return rect.getSize();
}



void Canvas::clear()
{
    img.create(sf::Vector2u(img.getSize().x, img.getSize().y));
}

sf::Color Canvas::getPixel(sf::Vector2f loc)
{
    if(validSpot(loc.x, loc.y))
        return img.getPixel(sf::Vector2u(loc.x, loc.y));
    return sf::Color();
}
void Canvas::drawPixel(sf::Vector2f pos)
{
    
    if(validSpot(pos.x, pos.y)){
        sf::Color current = getPixel(pos);

        /*
        int alpha = brushColor.a/255;
        int red   = std::clamp((current.r-alpha*current.r) * brushColor.r / 255, 0, 255);
        int green = std::clamp((current.g-alpha*current.g) * brushColor.g / 255, 0, 255);
        int blue  = std::clamp((current.b-alpha*current.b) * brushColor.b / 255, 0, 255);
         //*/
         
        
        if(!doColorMixing){
            img.setPixel(sf::Vector2u(pos.x, pos.y), brushColor);
        }else{
            //int alpha = 255 - ((255 - current.a) * (255 - brushColor.a) / 255);
            int red   = (current.r * (255 - brushColor.a) + brushColor.r * current.a) / 255;
            int green = (current.g * (255 - brushColor.a) + brushColor.g * current.a) / 255;
            int blue  = (current.b * (255 - brushColor.a) + brushColor.b * current.a) / 255;
            
            img.setPixel(sf::Vector2u(pos.x, pos.y), sf::Color(red, green, blue));
        }
        
        
    }
}

void Canvas::drawRect(sf::Vector2f pos, sf::Vector2f size)
{
    for(int a = 0; a < size.x; a++){
        drawPixel(c(pos.x+a, pos.y));
        drawPixel(c(pos.x+a, pos.y+pos.y-1));
    }
    for(int a = 0; a < size.y; a++){
        drawPixel(c(pos.x, pos.y+a));
        drawPixel(c(pos.x+size.x-1, pos.y+a));
    }
}

void Canvas::fillRect(sf::Vector2f pos, sf::Vector2f size)
{
    for(int column = 0; column < size.x; column++)
    {
        for(int row = 0; row < size.y; row++)
        {
            drawPixel(c(pos.x+column, pos.y+row));
        }
    }
}

void Canvas::fillSpace(sf::Vector2f pos)
{
    if(!validSpot(pos.x, pos.y))
        return;
    sf::Color allowedColor = getPixel(c(pos.x, pos.y));
    std::vector<sf::Vector2i> turtles;
    turtles.push_back(sf::Vector2i(pos.x, pos.y));
    
    if(brushColor == allowedColor)
        return;
    
    while(turtles.size() > 0)
    {
        if(validSpot(turtles[0]))
        {
            if(getPixel(c(turtles[0].x, turtles[0].y-1)) == allowedColor && turtles[0].y-1 >= 0)
            {
                turtles.push_back(sf::Vector2i(turtles[0].x, turtles[0].y-1));
                drawPixel(c(turtles[0].x, turtles[0].y-1));
            }
            if(getPixel(c(turtles[0].x+1, turtles[0].y)) == allowedColor && turtles[0].x+1 < img.getSize().x)
            {
                turtles.push_back(sf::Vector2i(turtles[0].x+1, turtles[0].y));
                drawPixel(c(turtles[0].x+1, turtles[0].y));
            }
            if(getPixel(c(turtles[0].x, turtles[0].y+1)) == allowedColor && turtles[0].y+1 < img.getSize().y)
            {
                turtles.push_back(sf::Vector2i(turtles[0].x, turtles[0].y+1));
                drawPixel(c(turtles[0].x, turtles[0].y+1));
            }
            if(getPixel(c(turtles[0].x-1, turtles[0].y)) == allowedColor && turtles[0].x-1 >= 0)
            {
                turtles.push_back(sf::Vector2i(turtles[0].x-1, turtles[0].y));
                drawPixel(c(turtles[0].x-1, turtles[0].y));
            }
            turtles.erase(turtles.begin());
        }
    }
}

void Canvas::drawLine(sf::Vector2f pos1, sf::Vector2f pos2)
{
    float lineLen = fmax(abs(pos2.x-pos1.x), abs(pos2.y-pos1.y));
    for(int i = 0; i < lineLen; i++)
    {
        float t = (float)i/lineLen;
        int x = round(pos1.x * (1.0-t) + pos2.x * t);
        int y = round(pos1.y * (1.0-t) + pos2.y * t);
        drawPixel(c(x, y));
    }
}


bool inside_circle(sf::Vector2f center, sf::Vector2i tile, float radius, float width) {
    float dx = center.x - tile.x,
    dy = center.y - tile.y;
    float distance = sqrt(dx*dx + dy*dy);
    
    if(width == -1)
        return (distance <= radius);
    else
        return (distance > radius-width && distance < radius+1);
}

void Canvas::drawCircle(sf::Vector2f pos, float r, float width)
{
    for (int yTile = pos.y-r; yTile <= pos.y+1+r; yTile++) {
        for (int xTile = pos.x-r; xTile <= pos.x+1+r; xTile++) {
            if(inside_circle(sf::Vector2f(pos.x, pos.y), sf::Vector2i(xTile, yTile), r, width)) {
                drawPixel(c(xTile, yTile));
            }
        }
    }
}

void Canvas::fillCircle(sf::Vector2f pos, float r)
{
    drawCircle(c(pos.x, pos.y), r, -1);
}

void Canvas::drawImg(sf::Vector2f pos, sf::Image& img)
{
    sf::Color col = brushColor;
    for(int xOff = 0; xOff < img.getSize().x; xOff++)
    {
        for(int yOff = 0; yOff < img.getSize().y; yOff++)
        {
            brushColor = img.getPixel(sf::Vector2u(xOff, yOff));
            if(!(brushColor.a < 2)){
                drawPixel(c(pos.x+xOff, pos.y+yOff));
            }
        }
    }
    brushColor = col;
}

sf::Vector2i Canvas::getMouseOverPixel(sf::RenderWindow &window)
{
    sf::Vector2i pos = sf::Mouse::getPosition(window);
    
    if(mouseOver){
        pos.x -= rect.getPosition().x;
        pos.y -= rect.getPosition().y;
        mousePosLastFrame = sf::Vector2i(pos.x/(rect.getSize().x/img.getSize().x),pos.y/(rect.getSize().y/img.getSize().y));
        return mousePosLastFrame;
        
    }
    
    if(doResetPosOnMouseOutsideBounds)
    {
        return sf::Vector2i(-1, -1);
    }
    
    return mousePosLastFrame;
}
