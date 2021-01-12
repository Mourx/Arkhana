#include "UnitZone.h"


UnitZone::UnitZone(int zoneType, ZONE_OWNER zPlayer) {
	if (zoneType == 0) {
		texIcon.loadFromFile("Textures/GUI/attackZone.png");
	}
	else {
		texIcon.loadFromFile("Textures/GUI/blockZone.png");
	}
	icon.setTexture(texIcon);

	type = zPlayer;
}

void UnitZone::Draw(RenderWindow* w) {
	w->draw(icon);
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

int UnitZone::GetCombinedMagicPower() {
	int power = 0;
	for (Unit* u : unitList) {
		power += u->GetMPower();
	}
	return power;
}

void UnitZone::AddUnit(Unit* u) {
	int count = unitList.size();
	u->SetPosition(this->GetIcon()->getPosition()+Vector2f(15 + (count%10)*45,5 + (count/10)*70));
	unitList.push_back(u);
	UpdateStatMods();
}

void UnitZone::UpdateStatMods() {
	zoneBonusPhys = 0;
	zoneBonusMag = 0;

	zoneMultiplierPhys = 0;
	zoneMultiplierMag = 0;

	for (Unit* u : unitList) {
		vector<Modifier*> auras = u->GetAuras();
		for (Modifier* mod : auras) {
			switch (mod->GetStat()) {
			case STAT_TYPE::DMG_PHYSICAL:
				zoneBonusPhys += mod->GetValue();
				zoneMultiplierPhys += mod->GetMultiplier();
				break;
			case STAT_TYPE::DMG_MAGIC:
				zoneBonusMag += mod->GetValue();
				zoneMultiplierMag += mod->GetMultiplier();
				break;
			default:
				break;
			}
		}
	}
	for (Unit* u : unitList) {
		u->SetZoneBonuses(zoneBonusPhys, zoneMultiplierPhys, zoneBonusMag, zoneMultiplierMag);
	}
}

void UnitZone::ModifyUnits(Modifier* mod) {
	for (Unit* u : unitList) {
		u->AddModifier(mod);
		u->UpdateStats();
		u->UpdateStrings();

	}
}