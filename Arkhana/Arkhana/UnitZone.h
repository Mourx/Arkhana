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
    UnitZone(int zoneType, Player* p,Player* enemy, ZONE_OWNER zPlayer, ZONE_TYPE t);
    ~UnitZone() {}
    vector<Unit*> GetUnits() { return unitList; }
    void Draw(RenderTexture* w);
    void DrawUnits(RenderTexture* w);
    void Update(Time t);
    int GetCombinedPhysicalPower();
    void AddUnit(Unit* u);
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
    ZONE_TYPE GetOppositeType() { 
        if (type == ZONE_TYPE::Z_ATTACK) {
            return ZONE_TYPE::Z_BLOCK;
        }
        else {
            return ZONE_TYPE::Z_ATTACK;
        }
    }
    Vector2f GetAnimationPoint() { return icon.getPosition() + animationPos; }
    void EndTurnUpkeep();
    void NewTurnUpkeep();
    int GetEffectCostChange();
    int GetUnitCostChange();
    void RemoveUnit(Unit* u);
    void AddMod(Modifier* mod);
    void RemoveMod(Modifier* mod);
    void RemoveMod(string modName);
    void SetEnemy(Player* e) {
        enemy = e;
    }
    void UpdatePositions();
    void SetTargetable(bool b) {
        bTargetable = b;
    }
    bool IsTargetable() {
        return bTargetable;
    }
    int GetEvasiveDamage();
protected:
    void UpdateStatMods();
    
    float zoneBonusPhys = 0;

    float zoneMultiplierPhys = 0;
    int zonePower = 0;

    bool bTargetable = false;

    vector<Unit*> unitList;
    ZONE_OWNER ownerType;
    Player* owner;
    Player* player;
    Player* enemy;
    Vector2f unitOffset = Vector2f(20, 40);
    Vector2f animationPos = Vector2f(50, 50);
    vector<Modifier*> zoneMods;
    ZONE_TYPE type;

    Sprite hoverIcon;
    Texture texHoverIcon;
    
    Shader shaderPulse;
    float pulseTimer = 0;

    Vector2f hoverOffset = Vector2f(0, 0);
};

