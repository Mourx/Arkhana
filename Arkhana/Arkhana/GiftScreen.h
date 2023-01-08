#pragma once
#include "Screen.h"
class Card;

class GiftScreen :
    public Screen
{
public:
    GiftScreen(RenderTexture* w, Player* p);
    ~GiftScreen() {}
    void Draw();
    void MouseMoved(Vector2f mousePos);
    void MouseClicked(Vector2f mousePos);

protected:
    void SetupGift();

    GameObject* nextButton;
    Player* player;
    Sprite background;
    Texture texBackground;
    Texture texButton;

    int _goldGift = 0;
    Card* _cardGift;
    //Treasure* _treasureGift;
};

