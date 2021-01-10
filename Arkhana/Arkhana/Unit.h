#pragma once
#include "GameObject.h"
#include "enums.h"
class Unit :
    public GameObject
{
public:
    Unit();
    ~Unit() {}
    float GetPPower() { return physicalPower; }
    float GetMPower() { return magicPower; }
    void SetPosition(Vector2f pos);
    void Draw(RenderWindow* w);
    void ModifyStat(STAT_TYPE stat, int value, int multiplier);
    void UpdateStrings();
protected:
    

    float physicalPower = 0;
    float magicPower = 0;

    float basePhys = 0;
    float baseMag = 0;
    String name = "";

    Sprite unitBackground;
    Texture texBackground;

    Text txtPhys;
    Text txtMag;

    Vector2f txtPhysPos = Vector2f(9, 50);
    Vector2f txtMagPos = Vector2f(31, 50);

    Font font;

};

