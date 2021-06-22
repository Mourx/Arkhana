#pragma once
#include "Screen.h"
#include "Card.h"
#include "Enemy.h"
class RewardScreen :
    public Screen
{
public:
    RewardScreen(RenderTexture* w,DataBase* db, Player* p,Enemy* e);
    ~RewardScreen(){}
    void Draw();
    void MouseMoved(Vector2f mousePos);
    void MouseClicked(Vector2f mousePos);
    void SetInfo(InfoPane* info);
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
    Vector2f optionPos = Vector2f(650, 380);

    Sprite goldIcon;
    Texture texGoldIcon;
    Vector2f goldIconPos = Vector2f(700, 630);

    Text txtGold;
    Font font;
    Vector2f goldTxtPos = Vector2f(820, 645);
};

