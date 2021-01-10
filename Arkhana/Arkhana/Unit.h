#pragma once
#include "GameObject.h"
class Unit :
    public GameObject
{
public:
    Unit();
    ~Unit() {}
    float GetPPower() { return physicalPower; }
    float GetMPower() { return magicPower; }
protected:
    float physicalPower;
    float magicPower;
    String name;
};

