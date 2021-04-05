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
protected:
    void GenerateOptions();
    void CreateDeckGrid();

    bool bUpgrading = false;

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
    Vector2f upgradeIconPos = Vector2f(1200, 550);

    Sprite screenShade;
    Texture texScreenShade;

    Vector2f upgradePos = Vector2f(400, 250);

    Sprite background;
    Texture texBackground;
};

