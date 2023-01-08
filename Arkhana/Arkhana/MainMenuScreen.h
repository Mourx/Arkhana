#pragma once
#include "Screen.h"
#include "Faction.h"
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
    void ShowAllCards();
    vector<Faction*> factions;
    Sprite background;
    Texture texBackground;

    Shader redShader;
    Shader blueShader;
    bool bShowAllCards = false;
    vector<Card*> cardGrid;
    Vector2f factionPosStart = Vector2f(1050, 650);

    Player* player;
};


