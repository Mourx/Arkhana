#pragma once
#include "GameObject.h"
#include "enums.h"
#include "UnitZone.h"
#include "DataBase.h"
#include <SFML/Audio.hpp>

class Player;

class Card :
    public GameObject
{
public:
    Card(CardData data,DataBase* dataB  );
    ~Card() {}

    Vector2f GetHandPos() { return handPos; }
    void SetHandPos(Vector2f p) { handPos = p; }
    int GetCost() { 
        int total = cost + costChange;
        if (total <= 0) {
            total = 0;
        }
        return total; 
    }
    virtual void Play(Unit* targUnit);
    virtual void Play(UnitZone* zone,Unit* targUnit);
    void SetPosition(Vector2f pos);
    void Draw(RenderTexture* w);
    void DrawCost(RenderTexture* w);
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
    int GetGoldCost() { return goldCost; }
    void AddModifier(Modifier* mod) { modifiers.push_back(mod); }
    void IncreaseStat(STAT_TYPE s, int val) {
        if (s == STAT_TYPE::DMG_PHYSICAL) {
            pPow += val;
        }
        if (s == STAT_TYPE::STAMINA) {
            stamina += val;
        }
        UpdateStrings();
    }
    void SetHover(bool b);
    String GetDesc() {
        return txtDesc.getString();
    }
    String GetName() {
        return txtName.getString();
    }
    bool IsUndercover() { return bUndercover; }
    bool IsPassive() { return bPassive; }
    void SetCostChange(int change);
    bool IsUnitTarget();
    void DoEffect(Unit* targUnit,UnitZone* targZone);
protected:
    
    virtual void ApplyModifier(UnitZone*);
    virtual void ApplyModifier(Unit*);
    void UpdatePositions();
    void UpdateStrings();
    DataBase* database;
    Vector2f handPos;
    string name;
    string unit;
    vector<Modifier*> modifiers;
    MODIFIER_TYPE mod;
    int cost = 0;
    int costChange = 0;
    int unitID;

    int stamina;
    int pPow;

    int goldCost = 0;
    bool bHasTargetZone = false;
    bool bUndercover = false;
    bool bPassive = false;
    
    UnitZone* targetZone;

    Shader cardShader;

    vector<Sprite> costIcons;
    Texture texCost;
    vector<Vector2f> costPositions = { Vector2f(139, 12) ,Vector2f(124, 12) ,Vector2f(109, 12),Vector2f(94, 12) };

    Text txtName;
    Text txtPhys;
    Text txtMag;
    Text txtDesc;
    Text txtGoldCost;

    Sprite arcanaSymbol;
    Texture texArcanaSymbol;

    Sprite physIcon;
    Texture texPhys;

    Sprite magIcon;
    Texture texMag;

    Sprite cardArt;
    Texture texCardArt;

    Sprite cardBorder;
    Texture texCardBorder;

    Font font;
    Font coolFont;

    Vector2f txtNamePos = Vector2f(75, 100);
    Vector2f txtPhysPos = Vector2f(110, 40);
    Vector2f physPos = Vector2f(135, 40);
    Vector2f txtMagPos = Vector2f(110, 65);
    Vector2f magPos = Vector2f(135, 65);
    Vector2f txtDescPos = Vector2f(75, 125);
    Vector2f cardArtPos = Vector2f(1, 1);
    Vector2f costIconPos = Vector2f(139, 12);
    Vector2f goldCostPos = Vector2f(75, 170);

    string zTag;
    AI_TAG AITag;
    EffectData* effect = NULL;
    CARD_TYPE type = UNIT;
    ZONE_OWNER zOwner = Z_PLAYER;
    ZONE_TYPE zType = ZONE_TYPE::Z_ANY;
};

