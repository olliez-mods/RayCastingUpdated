#ifndef Canvas_hpp
#define Canvas_hpp


#include "../Widget.hpp"
#include "../Conversion.hpp"

namespace sfas
{
class Canvas : public Widget
{
    bool mouseDownLastTick = true;
    
    sf::Image img;
    sf::Texture texture;
    sf::RectangleShape rect;
    sf::Vector2i mousePosLastFrame;
    
    bool validSpot(int x, int y);
    bool validSpot(sf::Vector2i pos);
public:
    bool doColorMixing = true;
    bool doResetPosOnMouseOutsideBounds = false;
    bool clickedOnTick = false;
    void setPosition(sf::Vector2f pos);
    void tick(sf::Window& window);
    void display(sf::RenderWindow& window);
    void checkEvent(sf::Event event);
    
    sf::Vector2i getSize();
    sf::Vector2f getBounds();

    void clear();
    sf::Color brushColor = sf::Color::Black;
    void drawPixel(sf::Vector2f pos);
    sf::Color getPixel(sf::Vector2f loc);
    void drawRect(sf::Vector2f pos, sf::Vector2f size);
    void fillRect(sf::Vector2f pos, sf::Vector2f size);
    void fillSpace(sf::Vector2f pos);
    void drawLine(sf::Vector2f pos1, sf::Vector2f pos2);
    void drawCircle(sf::Vector2f pos, float r, float width);
    void fillCircle(sf::Vector2f pos, float r);
    void drawImg(sf::Vector2f pos, sf::Image& img);
    
    sf::Vector2i getMouseOverPixel(sf::RenderWindow &window);
    
    
    void create(sf::Vector2f size);
    Canvas(sf::Vector2f pos, sf::Vector2f size);
    Canvas();
};
}

#endif
