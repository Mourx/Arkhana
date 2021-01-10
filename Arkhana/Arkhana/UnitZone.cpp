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
}

void UnitZone::ModifyUnits(Modifier* mod) {
	for (Unit* u : unitList) {
		u->ModifyStat(mod->GetStat(), mod->GetValue(), mod->GetMultiplier());
		u->UpdateStrings();
	}
}