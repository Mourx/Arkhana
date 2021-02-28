#pragma once
#include "GameObject.h"
#include "enums.h"
#include "UnitZone.h"
#include "DataBase.h"

class Player;

class Card :
    public GameObject
{
public:
    Card(CardData data,DataBase* dataB  );
    ~Card() {}

    Vector2f GetHandPos() { return handPos; }
    void SetHandPos(Vector2f p) { handPos = p; }
    int GetCost() { return cost; }
    virtual void Play();
    virtual void Play(UnitZone* zone);
    void SetPosition(Vector2f pos);
    void Draw(RenderWindow* w);
    CARD_TYPE GetType() { return type; }
    ZONE_OWNER GetZoneOwner() { return zOwner; }
    ZONE_TYPE GetZoneType() { return zType; }
    AI_TAG GetAITag() { return AITag; }
    void SetTarget(UnitZone* uz) {
        targetZone = uz;
        targetPos = uz->GetAnimationPoint();
        bMoving = true;
        xdir = (targetPos.x - pos.x) / moveDuration;
        ydir = (targetPos.y - pos.y) / moveDuration;
        
    }
    bool HasTargetZone() { return bHasTargetZone; }
    UnitZone* GetTargetZone() { return targetZone; }
protected:
    void DoEffect();
    virtual void ApplyModifier(UnitZone*);
    virtual void ApplyModifier(Unit*);
    void UpdatePositions();
    DataBase* database;
    Vector2f handPos;
    string name;
    string unit;
    vector<Modifier*> modifiers;
    MODIFIER_TYPE mod;
    int cost = 0;
    int unitID;

    int stamina;
    int pPow;

    bool bHasTargetZone = false;
    UnitZone* targetZone;

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

    Vector2f txtNamePos = Vector2f(85, 20);
    Vector2f txtPhysPos = Vector2f(20, 175);
    Vector2f physPos = Vector2f(40, 175);
    Vector2f txtMagPos = Vector2f(90, 175);
    Vector2f magPos = Vector2f(110, 175);
    Vector2f txtDescPos = Vector2f(75, 140);
    Vector2f cardArtPos = Vector2f(55, 50);
    Vector2f costIconPos = Vector2f(15, 15);

    string zTag;
    AI_TAG AITag;
    EffectData* effect = NULL;
    CARD_TYPE type = CREATE_UNIT;
    ZONE_OWNER zOwner = Z_PLAYER;
    ZONE_TYPE zType = ZONE_TYPE::Z_ANY;
};

