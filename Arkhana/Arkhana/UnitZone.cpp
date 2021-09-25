#include "UnitZone.h"
#include "Player.h"

UnitZone::UnitZone(int zoneType,Player* p,Player* e, ZONE_OWNER zPlayer, ZONE_TYPE t) {
	if (zoneType == 0) {
		texIcon.loadFromFile("Textures/GUI/attackZone.png");
	}
	else {
		texIcon.loadFromFile("Textures/GUI/blockZone.png");
	}
	icon.setTexture(texIcon);
	player = p;
	enemy = e;
	owner = zPlayer == ENEMY ? e : p;
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
		if(!u->IsUndercover()) power += u->GetPPower();
	}
	return power;
}


void UnitZone::AddMod(Modifier* mod) {
	zoneMods.push_back(mod);
}

void UnitZone::RemoveMod(Modifier* mod) {
	zoneMods.erase(remove(zoneMods.begin(), zoneMods.end(), mod), zoneMods.end());
}

void UnitZone::AddUnit(Unit* u, DataBase* database) {
	int count = unitList.size();
	vector<Modifier*> mods = u->GetModifiers();
	
	if(!u->IsBoss() && u->GetStaminaMod()== NULL) u->AddModifier((new Modifier(*database->modList["eot_stamina"])));
	unitList.push_back(u);
	for (Modifier* mod : u->GetModifiers()) {
		switch (mod->GetModType()) {
		case MODIFIER_TYPE::PLAYER_MOD:
		case MODIFIER_TYPE::PLAYER_EOT_MOD:
			player->AddMod(mod);
			break;
		case MODIFIER_TYPE::ZONE_MOD_ATTACK:
			owner->GetZones()[0]->AddMod(mod);
			break;
		default:
			break;
		}
	}
	player->GetZones()[(int)ZONE_TYPE::Z_ATTACK]->UpdatePositions();
	player->GetZones()[(int)ZONE_TYPE::Z_ATTACK]->UpdateStatMods();
	player->GetZones()[(int)ZONE_TYPE::Z_BLOCK]->UpdatePositions();
	player->GetZones()[(int)ZONE_TYPE::Z_BLOCK]->UpdateStatMods();
	enemy->GetZones()[0]->UpdatePositions();
	enemy->GetZones()[0]->UpdateStatMods();
	enemy->GetZones()[1]->UpdatePositions();
	enemy->GetZones()[1]->UpdateStatMods();
}

void UnitZone::RemoveUnit(Unit* u) {
	
	for (Modifier* mod : u->GetModifiers()) {
		switch (mod->GetModType()) {
		case MODIFIER_TYPE::PLAYER_MOD:
		case MODIFIER_TYPE::PLAYER_EOT_MOD:
			player->RemoveMod(mod);
			break;
		case MODIFIER_TYPE::ZONE_MOD_ATTACK:
			owner->GetZones()[0]->RemoveMod(mod);
			break;
		default:
			break;
		}
	}
	unitList.erase(remove(unitList.begin(),unitList.end(),u),unitList.end());

	player->GetZones()[(int)ZONE_TYPE::Z_ATTACK]->UpdatePositions();
	player->GetZones()[(int)ZONE_TYPE::Z_ATTACK]->UpdateStatMods();
	player->GetZones()[(int)ZONE_TYPE::Z_BLOCK]->UpdatePositions();
	player->GetZones()[(int)ZONE_TYPE::Z_BLOCK]->UpdateStatMods();
	enemy->GetZones()[0]->UpdatePositions();
	enemy->GetZones()[0]->UpdateStatMods();
	enemy->GetZones()[1]->UpdatePositions();
	enemy->GetZones()[1]->UpdateStatMods();
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
		for (Modifier* mod : zoneMods) {
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
			RemoveUnit(unitList[i]);
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

int UnitZone::GetEffectCostChange() {
	int total = 0;
	for (Unit* u : unitList) {
		for (Modifier* mod : u->GetModifiers()) {
			if (mod->GetModType() == MODIFIER_TYPE::PLAYER_MOD) {
				if (mod->GetStat() == STAT_TYPE::EFFECT_COST) {
					total += mod->GetValue();
				}
			}
		}
	}
	return total;
}

int UnitZone::GetUnitCostChange() {
	int total = 0;
	for (Unit* u : unitList) {
		for (Modifier* mod : u->GetModifiers()) {
			if (mod->GetModType() == MODIFIER_TYPE::PLAYER_MOD) {
				if (mod->GetStat() == STAT_TYPE::UNIT_COST) {
					total += mod->GetValue();
				}
			}
		}
	}
	return total;
}