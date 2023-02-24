#include "../dt.hpp"

namespace dtVars
{
    double dt = 0;
    sf::Clock deltaClock;
};


double resetDT()
{
    dtVars::dt = dtVars::deltaClock.restart().asSeconds();
    return dtVars::dt;
}

double getDT()
{
    return dtVars::dt;
}
