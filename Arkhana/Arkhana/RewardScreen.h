#pragma once
#include "Screen.h"
#include "Card.h"
#include "Enemy.h"
class RewardScreen :
    public Screen
{
public:
    RewardScreen(RenderWindow* w,DataBase* db, Player* p,Enemy* e);
    ~RewardScreen(){}
    void Draw();
    void MouseMoved(Vector2f mousePos);
    void MouseClicked(Vector2f mousePos);
protected:
    void GenerateOptions();

    Sprite background;
    Texture texBackground;

    Card* selCard;

    DataBase* database;
    vector<Card*> options;
    int moneyReward = 0;
    int optionsAmount = 3;
    Player* player;
    Enemy* enemy;
    Vector2f optionPos = Vector2f(500, 380);
};

