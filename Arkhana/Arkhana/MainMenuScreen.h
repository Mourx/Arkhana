#pragma once
#include "Screen.h"
#include "Faction.h"
#include "DataBase.h"
class Player;
class MainMenuScreen :
    public Screen
{
public:
    MainMenuScreen(RenderTexture* window,Player* p);
    ~MainMenuScreen(){}
    void Draw();
    void MouseMoved(Vector2f mousePos);
    void MouseClicked(Vector2f mousePos);
    void Reset() {
        nextScreen = NONE;
    }
protected:
    
    vector<Faction*> factions;
    DataBase* database;
    Sprite background;
    Texture texBackground;

    Shader redShader;
    Shader blueShader;

    Vector2f factionPosStart = Vector2f(1050, 650);

    Player* player;
};


