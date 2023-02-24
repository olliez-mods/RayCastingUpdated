#ifndef Input_hpp
#define Input_hpp
#include "../Widget.hpp"

namespace sfas
{
class Input : public Widget
{
    bool mouseDownLastTick = true;
    sf::Text stringText;
    std::vector<int> keysPressedThisTick;
    std::string value;

    sf::Texture rectTexture;
public:
    bool inFocus = false;
    bool clickedOnTick = false;
    sf::RectangleShape rect;
    void setPosition(sf::Vector2f pos);
    void tick(sf::Window& window);
    void setTextSize(int textSize);
    std::string getText();
    void setText(std::string text);
    void setTexture(sf::Texture texture);
    void setTexture(std::string texture);
    void setTextColor(sf::Color color);
    void setColor(sf::Color color);
    void display(sf::RenderWindow& window);
    void checkEvent(sf::Event event);
    Input(sf::Vector2f pos, sf::Vector2f size, sf::Color color = sf::Color::White);
    Input();
};
}
#endif
