#pragma once
#include "GameObject.h"
#include "Modifier.h"
#include "enums.h"
#include <time.h>
#include <vector>
using namespace std;

class Card;

class Unit :
    public GameObject
{
public:
    Unit(UnitData data, vector<Modifier*> mods,Card* c);
    ~Unit() {}
    float GetPPower() { return physicalPower; }
    float GetStamina() { return stamina; }
    void SetPosition(Vector2f pos);
    void Draw(RenderTexture* w);
    void Update(Time t);
    void UpdateStats();
    void AddModifier(Modifier* mod);
    void SetZoneBonusesPhys(float phys, float physM, vector<Modifier*> zMods) {
        zoneBonusPhys = phys;
        zoneMultiplierPhys = physM;
        zoneMods = zMods;
        UpdateStats();
        
    }
    void UpdateStrings();
    vector<Modifier*> GetAuras() { return auraMods; }
    void Move(Vector2f offset);
    vector<Modifier*> GetModifiers() { return unitMods; }
    vector<Modifier*> GetZoneModifiers() { return zoneMods; }
    Card* GetCard() {
        return card;
    }
    bool IsBoss() { return bBoss; }
    bool IsUndercover();
    void RemoveUnitMod(Modifier* mod);
    Modifier* GetStaminaMod();
    void Retreat() {
        bRetreating = true;
    }
    void Attack() {
        bAttacking = true;
    }
    bool GetRetreated() { return bHasRetreated; }
    void SetDirections(int atk, int ret) {
        retreatDirection = ret;
        attackDirection = atk;
    }
protected:
    void ModifyStat(STAT_TYPE stat, int value, int multiplier);
    
    float physicalPower = 0;
    float stamina = 0;
    
    float zoneBonusPhys = 0;
    float zoneBonusMag = 0;
    float zoneMultiplierPhys = 0;
    float zoneMultiplierMag = 0;

    float nudgeTimer = rand() % 32 / 200.f;
    float nudgeDelay = 0.25;
    int nudgeDir = 1;

    Card* card;
    float basePhys = 0;
    float baseStamina = 0;
    String name = "";

    bool bBoss = false;


    float attackDuration = 2;
    float attackTimer = 0;
    float attackDirection = -1;
    bool bAttacking = false;
    bool bHasAttacked = false;

    bool bRetreating = false;
    bool bHasRetreated = false;
    float retreatDuration = 1;
    float retreatTimer = 0;
    float retreatDirection = 1;

    Sprite unitBackground;
    Texture texBackground;

    Sprite highlightIcon;
    Texture texHighlight;

    Text txtPhys;
    Text txtMag;

    Vector2f txtPhysPos = Vector2f(20, 100);
    Vector2f txtMagPos = Vector2f(60, 100);

    Font font;

    vector<Modifier*> unitMods;
    vector<Modifier*> zoneMods;
    vector<Modifier*> auraMods;

};

