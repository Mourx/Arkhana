#pragma once
#include "GameObject.h"
class Card :
    public GameObject
{
public:
    Card(int ID);
    ~Card() {}

    Vector2f GetHandPos() { return handPos; }
    void SetHandPos(Vector2f p) { handPos = p; }
    int GetCost() { return cost; }
    virtual void Play() {}
protected:
    Vector2f handPos;

    int cost = 0;
};

