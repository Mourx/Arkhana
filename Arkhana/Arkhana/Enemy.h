#pragma once
#include "Player.h"


class Enemy :
    public Player
{
public:
    Enemy() {}
    Enemy(RenderTexture* w,DataBase* data);
    ~Enemy(){}
    Card* GetNext() { return decklist[cardIndex]; }
    Card* PlayNext();
    void DrawActions();
    void NewTurnUpkeep();
    void EndTurnUpkeep();
    void SetDetails(EncounterData* data, vector<Card*> startPlay, vector<vector<Card*>> dl);
    void Update(Time t);
    void SetNextMove();
    vector<Card*> GetStartingPlay() { return startingPlay; }
    int GetLevel() { return level; }
protected:

    void InitSprites();

    int maxArmour = 0;
    int cardIndex = 0;
    int level = 0;
    vector<vector<Card*>> decklists;
    vector<Card*> deck;
    vector<Card*> startingPlay;

    Vector2f eNextPos = Vector2f(25, 125);

};

