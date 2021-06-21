#pragma once
#include "Screen.h"
#include "Player.h"
#include "Encounter.h"
#include "ForgeScreen.h"

class PathScreen :
    public Screen
{       
public:
    PathScreen(RenderWindow* w, DataBase* db,Player* p);
    ~PathScreen() {}
    void Draw();
    void Update(Time t);
    Encounter* GetEncounter() { return currentEncounter; }
    void MouseMoved(Vector2f mousePos);
    void MouseClicked(Vector2f mousePos);
    void MouseReleased(Vector2f mousePos);
    void ResetDetails(COMBAT_RESULT res);
    ForgeScreen* GetForge() { return forge; }
    void SetInfo(InfoPane* info);
    void ForgeVisited(bool b) {
        bForgeVisited = b;
    }
protected:
    void InitEncounters();

    RenderWindow* window;
    DataBase* database;
    Player* player;
    
    ForgeScreen* forge;
    GameObject* forgeIcon;
    Texture texForgeIcon;

    string forgePrompt;
    string forgeVisitedPrompt;

    Text txtForgeUnlocks;
    Font font;

    Vector2f forgeIconPos = Vector2f(15, 600);

    Encounter* currentEncounter = NULL;
    vector<Encounter*> encounters;

    vector<int> encounterAmounts = { 3,2,1 };

    bool bForgeUnlocked = true;
    bool bForgeVisited = true;

    int tier = 0;
    int forgeUnlocks = 0;

    vector<Vector2f> encPos = { Vector2f(200, 330),Vector2f(525, 380),Vector2f(850, 580) };

    Sprite background;
    Texture texBackground;
};

