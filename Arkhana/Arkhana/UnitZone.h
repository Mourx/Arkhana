#pragma once
#include "GameObject.h"
#include "Unit.h"
#include "enums.h"
#include "Modifier.h"
#include <vector>

using namespace std;
class Player;
class UnitZone :
    public GameObject
{
public:
    UnitZone(int zoneType, Player* own, ZONE_OWNER zPlayer, ZONE_TYPE t);
    ~UnitZone() {}
    vector<Unit*> GetUnits() { return unitList; }
    void Draw(RenderTexture* w);
    void DrawUnits(RenderTexture* w);
    int GetCombinedPhysicalPower();
    void AddUnit(Unit* u,DataBase* database);
    void ClearUnits() { unitList.clear(); }
    ZONE_OWNER GetOwnerType() { return ownerType; }
    Player* GetOwner() { return owner; }
    void ModifyUnits(Modifier* mod);
    void CheckStamina();
    int GetStrongestPower() {
        int power = -999;
        for (Unit* u : unitList) {
            if (u->GetPPower() > power) {
                power = u->GetPPower();
            }
        }
        return power;
    }
    ZONE_TYPE GetType() { return type; }
    Vector2f GetAnimationPoint() { return icon.getPosition() + animationPos; }
    void EndTurnUpkeep(DataBase* database);
    void NewTurnUpkeep(DataBase* database);
protected:
    void UpdateStatMods();
    void UpdatePositions() {
        int offset = 0;
        if (this->ownerType == ENEMY) {
            offset = 250;
        }
        for (int i = 0; i < unitList.size(); i++) {
            unitList[i]->SetPosition(this->GetIcon()->getPosition() + Vector2f(10 + (i % 10) * 50, 10 + offset + (i / 10) * 75));
        }
    }
    float zoneBonusPhys = 0;

    float zoneMultiplierPhys = 0;

    vector<Unit*> unitList;
    ZONE_OWNER ownerType;
    Player* owner;
    Vector2f unitOffset = Vector2f(20, 20);
    Vector2f animationPos = Vector2f(50, 50);
    vector<Modifier*> zoneMods;
    ZONE_TYPE type;
};

