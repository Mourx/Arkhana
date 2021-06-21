#pragma once
#include "Screen.h"
#include "Player.h"
class ForgeScreen :
    public Screen
{
public:
    ForgeScreen(RenderWindow* w, DataBase* db, Player* p);
    ~ForgeScreen() {};
    void Draw();
    void MouseMoved(Vector2f mousePos);
    void MouseClicked(Vector2f mousePos);
    void ResetNextScreen() { nextScreen = NONE; }
    void Update(Time t);
protected:
    void GenerateOptions();
    void CreateDeckGrid();

    bool bUpgrading = false;
    bool bSelectUpgrade = false;

    Card* selUpgCard;
    Card* selCard;
    
    Player* player;
    DataBase* database;

    vector<Card*> options;
    int optionsAmount = 6;

    vector<Vector2f> optionsPos = {
        Vector2f(150,200), Vector2f(400,200),
        Vector2f(150,470), Vector2f(400,470),
        Vector2f(150,740), Vector2f(400,740),
    };
    vector<Text> optionCostsTxt;
    Font font;

    GameObject* pathIcon;
    Texture texPathIcon;

    GameObject* upgradeIcon;
    Texture texUpgradeIcon;
    Vector2f upgradeIconPos = Vector2f(1300, 200);

    GameObject* upgAttack;
    Texture texUpgAttack;
    Vector2f upgAttackPos = Vector2f(550, 250);

    GameObject* upgStamina;
    Texture texUpgStamina;
    Vector2f upgStaminaPos = Vector2f(950, 250);

    Sprite screenShade;
    Texture texScreenShade;

    Vector2f upgradePos = Vector2f(400, 250);
    
    Text txtUpgradeCost;
    Vector2f txtUpgradeCostPos = upgradeIconPos + Vector2f(100, 200);

    int upgradeCost = 75;

    Sprite background;
    Texture texBackground;
};

