#pragma once
#include "GameObject.h"
#include "Modifier.h"
#include "enums.h"
#include <vector>
using namespace std;

class Unit :
    public GameObject
{
public:
    Unit(UnitData data, vector<Modifier*> mods);
    ~Unit() {}
    float GetPPower() { return physicalPower; }
    float GetMPower() { return magicPower; }
    void SetPosition(Vector2f pos);
    void Draw(RenderWindow* w);
    void UpdateStats();
    void AddModifier(Modifier* mod);
    void SetZoneBonuses(float phys, float physM, float mag, float magM) { 
        zoneBonusPhys = phys;
        zoneBonusMag = mag;
        zoneMultiplierPhys = physM;
        zoneMultiplierMag = magM;
        UpdateStats();
        
    }
    void UpdateStrings();
    vector<Modifier*> GetAuras() { return auraMods; }
protected:
    void ModifyStat(STAT_TYPE stat, int value, int multiplier);
    
    float physicalPower = 0;
    float magicPower = 0;

    float zoneBonusPhys = 0;
    float zoneBonusMag = 0;
    float zoneMultiplierPhys = 0;
    float zoneMultiplierMag = 0;


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

    vector<Modifier*> unitMods;
    vector<Modifier*> auraMods;
};

