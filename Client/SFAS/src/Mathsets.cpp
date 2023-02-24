#include "../Mathsets.hpp"
#include <iostream>

SetMap::SetMap()
{
    setOneStart = 0;
    setOneEnd = 1;
    setTwoStart = 0;
    setTwoEnd = 1;
}

SetMap::SetMap(float setOneStart, float setOneEnd, float setTwoStart, float setTwoEnd)
{
    this->setOneStart = setOneStart;
    this->setOneEnd = setOneEnd;
    this->setTwoStart = setTwoStart;
    this->setTwoEnd = setTwoEnd;
}
float SetMap::getValue(float value)
{
    //Dont ask me how this works xD
    return((value-setOneStart)/(setOneEnd-setOneStart) * (setTwoEnd-setTwoStart) + setTwoStart);
}
