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
    void NewTurnUpkeep();
    void SetDetails(EncounterData* data, vector<Card*> startPlay, vector<Card*> decklist);
protected:

    void InitSprites();

    int maxArmour = 0;
    int cardIndex = 0;
    vector<Card*> deck;
    vector<Card*> startingPlay;

};

