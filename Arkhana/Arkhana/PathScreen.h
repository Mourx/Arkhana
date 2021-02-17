#pragma once
#include "Screen.h"
#include "Player.h"
#include "Encounter.h"
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
protected:
    void InitEncounters();

    RenderWindow* window;
    DataBase* database;
    Player* player;

    Encounter* currentEncounter = NULL;
    vector<Encounter*> encounters;

    vector<int> encounterAmounts = { 3,2,1 };

    vector<Vector2f> encPos = { Vector2f(200, 150),Vector2f(700, 200),Vector2f(1100, 400) };

    Sprite background;
    Texture texBackground;
};

