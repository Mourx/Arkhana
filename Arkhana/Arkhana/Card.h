#pragma once
#include "GameObject.h"
#include "enums.h"
#include "UnitZone.h"

class Card :
    public GameObject
{
public:
    Card(int ID);
    ~Card() {}

    Vector2f GetHandPos() { return handPos; }
    void SetHandPos(Vector2f p) { handPos = p; }
    int GetCost() { return cost; }
    virtual void Play(UnitZone* zone);
    CARD_TYPE GetType() { return type; }
    ZONE_OWNER GetZones() { return zOwner; }
protected:
    Vector2f handPos;

    virtual void ApplyModifier(UnitZone*);

    Modifier* modifier;
    MODIFIER_TYPE mod;
    int cost = 0;
    int unitID;

    CARD_TYPE type = UNIT;
    ZONE_OWNER zOwner = Z_PLAYER;
};

