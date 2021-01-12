#pragma once
#include "GameObject.h"
#include "Unit.h"
#include "enums.h"
#include "Modifier.h"
#include <vector>

using namespace std;

class UnitZone :
    public GameObject
{
public:
    UnitZone(int zoneType, ZONE_OWNER zPlayer);
    ~UnitZone() {}
    vector<Unit*> GetUnits() { return unitList; }
    void Draw(RenderWindow* w);
    int GetCombinedPhysicalPower();
    int GetCombinedMagicPower();
    void AddUnit(Unit* u);
    ZONE_OWNER GetOwner() { return type; }
    void ModifyUnits(Modifier* mod);
protected:
    void UpdateStatMods();

    float zoneBonusPhys = 0;
    float zoneBonusMag = 0;

    float zoneMultiplierPhys = 0;
    float zoneMultiplierMag = 0;

    vector<Unit*> unitList;
    ZONE_OWNER type;
    Vector2f unitOffset = Vector2f(20, 20);
    vector<Modifier*> zoneMods;
};

