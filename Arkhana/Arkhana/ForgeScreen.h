#pragma once
#include "Screen.h"
#include "Player.h"
class ForgeScreen :
    public Screen
{
public:
    ForgeScreen(RenderTexture* w, Player* p);
    ~ForgeScreen() {};
    void Draw();
    void MouseMoved(Vector2f mousePos);
    void MouseClicked(Vector2f mousePos);
    void ResetNextScreen() { nextScreen = NONE; }
    void Update(Time t);
protected:
    void GenerateOptions();
    void CreateDeckGrid(bool bUpgrading);

    bool bUpgrading = false;
    bool bSelectUpgrade = false;
    bool bSelectRemove = false;

    Card* selUpgCard;
    Card* selCard;
    
    Player* player;

    vector<Card*> options;
    int optionsAmount = 6;

    vector<Vector2f> optionsPos = {
        Vector2f(150,20), Vector2f(400,20),
        Vector2f(150,290), Vector2f(400,290),
        Vector2f(150,560), Vector2f(400,560),
    };
    vector<Text> optionCostsTxt;
    Font* font;

    GameObject* pathIcon;
    Texture texPathIcon;

    GameObject* removeIcon;
    Texture texRemoveIcon;
    Vector2f removeIconPos = Vector2f(1300, 600);

    GameObject* upgradeIcon;
    Texture texUpgradeIcon;
    Vector2f upgradeIconPos = Vector2f(1300, 20);

    GameObject* upgAttack;
    Texture texUpgAttack;
    Vector2f upgAttackPos = Vector2f(550, 70);

    GameObject* upgStamina;
    Texture texUpgStamina;
    Vector2f upgStaminaPos = Vector2f(950, 70);

    Sprite screenShade;
    Texture texScreenShade;

    Vector2f upgradePos = Vector2f(400, 70);
    
    Text txtUpgradeCost;
    Vector2f txtUpgradeCostPos = upgradeIconPos + Vector2f(100, 200);

    Text txtRemoveCost;
    Vector2f txtRemoveCostPos = removeIconPos + Vector2f(100, 200);

    int upgradeCost = 75;
    int removeCost = 75;

    Sprite background;
    Texture texBackground;
};

