#pragma once
#include "Screen.h"
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
protected:
    GameObject* faction1;
    
    Texture texFaction1;
    
    Sprite background;
    Texture texBackground;

    Shader redShader;
    Shader blueShader;

    Vector2f faction1Pos = Vector2f(1250, 650);

    Player* player;
};


