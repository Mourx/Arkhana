#include "UnitZone.h"


UnitZone::UnitZone() {

}

void UnitZone::Draw(RenderWindow* w) {
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