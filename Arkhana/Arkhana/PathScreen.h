#pragma once
#include "Screen.h"
#include "Player.h"
#include "Encounter.h"
#include "ForgeScreen.h"

class PathScreen :
    public Screen
{       
public:
    PathScreen(RenderTexture* w, DataBase* db,Player* p);
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

    RenderTexture* window;
    DataBase* database;
    Player* player;
    
    ForgeScreen* forge;
    GameObject* forgeIcon;
    Texture texForgeIcon;

    string forgePrompt;
    string forgeVisitedPrompt;

    Text txtForgeUnlocks;
    Font font;

    Vector2f forgeIconPos = Vector2f(15, 420);

    Encounter* currentEncounter = NULL;
    vector<Encounter*> encounters;

    vector<int> encounterAmounts = { 3,3,2,1 };

    bool bForgeUnlocked = true;
    bool bForgeVisited = true;

    int tier = 0;
    int forgeUnlocks = 0;

    vector<Vector2f> encPos = { Vector2f(200, 150),Vector2f(525, 150),Vector2f(850, 200),Vector2f(1150, 480) };

    Sprite background;
    Texture texBackground;
};

