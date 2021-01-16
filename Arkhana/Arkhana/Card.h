#pragma once
#include "GameObject.h"
#include "enums.h"
#include "UnitZone.h"
#include "DataBase.h"

class Card :
    public GameObject
{
public:
    Card(CardData data,DataBase* dataB  );
    ~Card() {}

    Vector2f GetHandPos() { return handPos; }
    void SetHandPos(Vector2f p) { handPos = p; }
    int GetCost() { return cost; }
    virtual void Play(UnitZone* zone);
    void SetPosition(Vector2f pos);
    void Draw(RenderWindow* w);
    CARD_TYPE GetType() { return type; }
    ZONE_OWNER GetZones() { return zOwner; }
protected:
    
    virtual void ApplyModifier(UnitZone*);
    void UpdatePositions();
    DataBase* database;
    Vector2f handPos;
    string name;
    string unit;
    vector<Modifier*> modifiers;
    MODIFIER_TYPE mod;
    int cost = 0;
    int unitID;

    int mPow;
    int pPow;

    Shader cardShader;

    Sprite costIcon;
    Texture texCost;

    Text txtName;
    Text txtPhys;
    Text txtMag;
    Text txtDesc;

    Sprite arcanaSymbol;
    Texture texArcanaSymbol;

    Sprite physIcon;
    Texture texPhys;

    Sprite magIcon;
    Texture texMag;

    Sprite cardArt;
    Texture texCardArt;

    Font font;

    Vector2f txtNamePos = Vector2f(75, 20);
    Vector2f txtPhysPos = Vector2f(20, 175);
    Vector2f physPos = Vector2f(40, 175);
    Vector2f txtMagPos = Vector2f(70, 175);
    Vector2f magPos = Vector2f(90, 175);
    Vector2f txtDescPos = Vector2f(75, 140);
    Vector2f cardArtPos = Vector2f(55, 50);
    Vector2f costIconPos = Vector2f(15, 15);


    CARD_TYPE type = UNIT;
    ZONE_OWNER zOwner = Z_PLAYER;
};

