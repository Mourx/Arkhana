#pragma once
#include "Screen.h"
#include "Card.h"
#include "Player.h"

class CombatScreen :
    public Screen
{
public:
    CombatScreen(RenderWindow* w);
    ~CombatScreen() {}
    void Draw();
protected:
    Player* player;
};

