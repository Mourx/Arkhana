#include "UnitZone.h"


UnitZone::UnitZone(int zoneType,Player* p, ZONE_OWNER zPlayer, ZONE_TYPE t) {
	if (zoneType == 0) {
		texIcon.loadFromFile("Textures/GUI/attackZone.png");
	}
	else {
		texIcon.loadFromFile("Textures/GUI/blockZone.png");
	}
	icon.setTexture(texIcon);

	owner = p;
	ownerType = zPlayer;
	type = t;
}

void UnitZone::Draw(RenderTexture* w) {
	w->draw(icon);
	
}

void UnitZone::DrawUnits(RenderTexture* w) {
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


void UnitZone::AddUnit(Unit* u, DataBase* database) {
	int count = unitList.size();
	vector<Modifier*> mods = u->GetModifiers();
	
	if(!u->IsBoss()) u->AddModifier((new Modifier(*database->modList["eot_stamina"])));
	

	unitList.push_back(u);
	UpdatePositions();
	UpdateStatMods();
}

void UnitZone::UpdateStatMods() {
	zoneBonusPhys = 0;

	zoneMultiplierPhys = 0;
	vector<Modifier*> zMods;
	for (Unit* u : unitList) {
		vector<Modifier*> auras = u->GetAuras();
		for (Modifier* mod : auras) {
			zMods.push_back(mod);
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
		u->SetZoneBonuses(zoneBonusPhys, zoneMultiplierPhys,zMods);
	}
}

void UnitZone::EndTurnUpkeep(DataBase* database) {
	
	for (Unit* u : unitList) {
		if (type == ZONE_TYPE::Z_ATTACK) {
		}
		for (Modifier* mod : u->GetModifiers()) {
			if (mod->GetModType() == MODIFIER_TYPE::UNIT_EOT_MOD) {
				u->AddModifier(mod->GetModifier());
			}
			mod->ApplyEOT();
		}
		
		for (Modifier* mod : u->GetAuras()) {
			if (mod->GetModType() == MODIFIER_TYPE::AURA_EOT_MOD) {
				for (Unit* un : unitList) {
					un->AddModifier(mod->GetModifier());
				}
			}
			
		}
		u->UpdateStats();
	}
	CheckStamina();
}


void UnitZone::NewTurnUpkeep(DataBase* database) {
	for (Unit* u : unitList) {
		if (type == ZONE_TYPE::Z_BLOCK) {
			u->UpdateStats();
		}
	}
	
	CheckStamina();
}

void UnitZone::CheckStamina() {

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