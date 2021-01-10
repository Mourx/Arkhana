#pragma once
#include "GameObject.h"
#include "Unit.h"
#include <vector>

using namespace std;

class UnitZone :
    public GameObject
{
public:
    UnitZone();
    ~UnitZone() {}
    vector<Unit*> GetUnits() { return unitList; }
    void Draw(RenderWindow* w);
    int GetCombinedPhysicalPower();
    int GetCombinedMagicPower();
protected:
    vector<Unit*> unitList;
};

