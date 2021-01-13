#pragma once
#include "Player.h"


class Enemy :
    public Player
{
public:
    Enemy(RenderWindow* w,DataBase* data);
    ~Enemy(){}
    Card* GetNext() { return deck[cardIndex]; }
    Card* PlayNext();
    void Draw();
protected:

    void InitSprites();

    int cardIndex = 0;
    vector<Card*> deck;

};

