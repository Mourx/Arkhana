#pragma once
#include "Screen.h"
#include "Card.h"
class RewardScreen :
    public Screen
{
public:
    RewardScreen(RenderWindow* w,DataBase* db);
    ~RewardScreen(){}

protected:
    void GenerateOptions();
    
    DataBase* database;
    vector<Card*> options;
    int moneyReward = 0;


    Vector2f optionPos = Vector2f(200, 200);
};

