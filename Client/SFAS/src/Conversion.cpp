#include "../Conversion.hpp"
using namespace sfas;
sf::Vector2f out = sf::Vector2f(0, 0);

sf::Vector2f sfas::c(float x, float y)
{
    out.x = x;
    out.y = y;
    return out;
}

sf::Vector2f sfas::c(float n)
{
    out.x = n;
    out.y = n;
    return out;
}

sf::Vector2f sfas::c(sf::Vector2i in)
{
    out.x = in.x;
    out.y = in.y;
    return out;
}

