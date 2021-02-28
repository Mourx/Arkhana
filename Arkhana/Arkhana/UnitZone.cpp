#include "UnitZone.h"


UnitZone::UnitZone(int zoneType, ZONE_OWNER zPlayer, ZONE_TYPE t) {
	if (zoneType == 0) {
		texIcon.loadFromFile("Textures/GUI/attackZone.png");
	}
	else {
		texIcon.loadFromFile("Textures/GUI/blockZone.png");
	}
	icon.setTexture(texIcon);

	owner = zPlayer;
	type = t;
}

void UnitZone::Draw(RenderWindow* w) {
	w->draw(icon);
	
}

void UnitZone::DrawUnits(RenderWindow* w) {
	for (Unit* u : unitList) {
		u->Draw(w);
	}
}

int UnitZone::GetCombinedPhysicalPower() {
	int power = 0;
	for (Unit* u : unitList) {
		power += u->GetPPower();
	}
	return power;
}


void UnitZone::AddUnit(Unit* u) {
	int count = unitList.size();
	unitList.push_back(u);
	UpdatePositions();
	UpdateStatMods();
}

void UnitZone::UpdateStatMods() {
	zoneBonusPhys = 0;

	zoneMultiplierPhys = 0;

	for (Unit* u : unitList) {
		vector<Modifier*> auras = u->GetAuras();
		for (Modifier* mod : auras) {
			switch (mod->GetStat()) {
			case STAT_TYPE::DMG_PHYSICAL:
				zoneBonusPhys += mod->GetValue();
				zoneMultiplierPhys += mod->GetMultiplier();
				break;
			case STAT_TYPE::STAMINA:
				
				break;
			default:
				break;
			}
		}
	}
	for (Unit* u : unitList) {
		u->SetZoneBonuses(zoneBonusPhys, zoneMultiplierPhys);
	}
}

void UnitZone::LowerStamina() {

	for (int i = 0; i < unitList.size();i++) {
		if (unitList[i]->GetStamina() <= 0) {
			unitList.erase(unitList.begin() + i);
			i--;
		}
	}
	UpdatePositions();
}

void UnitZone::ModifyUnits(Modifier* mod) {
	for (Unit* u : unitList) {
		u->AddModifier(mod);
		u->UpdateStats();
		u->UpdateStrings();

	}
}