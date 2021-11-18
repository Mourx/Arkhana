#pragma once
#include "Screen.h"
class GameOverScreen :
    public Screen
{
public:
    GameOverScreen(RenderTexture* w, DataBase* db, Player* p);
    ~GameOverScreen() {}
    void Draw();
    void MouseMoved(Vector2f mousePos);
    void MouseClicked(Vector2f mousePos);
    void ResetNextScreen() { nextScreen = NONE; }
    void Update(Time t);

protected:
    DataBase* database;
    Player* player;

    GameObject* mainMenuButton;
    Texture texMenuButton;

    Sprite background;
    Texture texBackground;
};