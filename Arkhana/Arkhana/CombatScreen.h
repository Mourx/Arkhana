#pragma once
#include "Screen.h"
#include "Card.h"
#include "Player.h"
#include "EndTurnButton.h"

class CombatScreen :
    public Screen
{
public:
    CombatScreen(RenderWindow* w);
    ~CombatScreen() {}
    void Draw();
    void Update(Time t);
    void MouseMoved(Vector2f mousePos);
    void MouseClicked(Vector2f mousePos);
    void MouseReleased(Vector2f mousePos);
protected:
    Player* player;
    
    void AdvanceTurn();

    float iconOffsetX = 0;
    float iconOffsetY = 0;
    Vector2f iconSnapPos;

    EndTurnButton* endTurn;
    
};

