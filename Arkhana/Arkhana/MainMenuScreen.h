#pragma once
#include "Screen.h"
class Player;
class MainMenuScreen :
    public Screen
{
public:
    MainMenuScreen(RenderWindow* window,Player* p);
    ~MainMenuScreen(){}
    void Draw();
    void MouseMoved(Vector2f mousePos);
    void MouseClicked(Vector2f mousePos);
protected:
    Sprite redFactionIcon;
    Sprite blueFactionIcon;
    
    Texture texRedFaction;
    Texture texBlueFaction;
    
    Shader redShader;
    Shader blueShader;

    Vector2f redFactionPos = Vector2f(600, 400);
    Vector2f blueFactionPos = Vector2f(900, 400);

    Player* player;
};


