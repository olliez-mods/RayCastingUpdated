#ifndef Mathsets_hpp
#define Mathsets_hpp

class SetMap
{
    float setOneStart, setOneEnd, setTwoStart, setTwoEnd;
public:
    SetMap();
    SetMap(float setOneStart, float setOneEnd, float setTwoStart, float setTwoEnd);
    float getValue(float value);
};

#endif
