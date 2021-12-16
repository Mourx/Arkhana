#include "UnitZone.h"
#include "Player.h"

UnitZone::UnitZone(int zoneType,Player* p,Player* e, ZONE_OWNER zPlayer, ZONE_TYPE t) {
	if (zoneType == 0) {
		texIcon.loadFromFile("Textures/GUI/attackZone.png");
		texHoverIcon.loadFromFile("Textures/GUI/attackZoneHover.png");
	}
	else {
		texIcon.loadFromFile("Textures/GUI/blockZone.png");
		texHoverIcon.loadFromFile("Textures/GUI/blockZoneHover.png");
	}
	shaderPulse.loadFromFile("Textures/Shaders/pulse.vert", Shader::Vertex);

	icon.setTexture(texIcon);
	hoverIcon.setTexture(texHoverIcon);
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
	if (GetHover()) {
		w->draw(hoverIcon);
	}
	else if (bTargetable) {
		w->draw(hoverIcon,&shaderPulse);
	}
	
}

int UnitZone::GetCombinedPhysicalPower() {
	int power = 0;
	for (Unit* u : unitList) {
		if(!u->IsUndercover() && !u->IsPassive()) power += u->GetPPower();
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
	if (ownerType == ENEMY) {
		u->SetDirections(1, -1);
	}
	if(!u->IsBoss() && u->GetStaminaMod()== NULL) u->AddModifier((new Modifier(*database->modList["eot_stamina"])));
	for (Unit* unit : GetUnits()) {
		for (Modifier* mod : unit->GetModifiers()) {
			if (mod->GetModType() == MODIFIER_TYPE::UNIT_ENTER_APPLY_MOD) {
				u->AddModifier(mod->GetModifier());
			}
		}
	}
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
		case::MODIFIER_TYPE::OPPOSITE_ZONE:
			if (this->type == ZONE_TYPE::Z_ATTACK) {
				Player* p;
				if (this->ownerType == ENEMY) {
					p = player;
				}
				else {
					p = enemy;
				}
				for (Unit* unit : p->GetZones()[1]->GetUnits()) {
					unit->AddModifier(mod);
				}
			}
			else {
				Player* p;
				if (this->ownerType == ENEMY) {
					p = player;
				}
				else {
					p = enemy;
				}
				for (Unit* unit : p->GetZones()[0]->GetUnits()) {
					unit->AddModifier(mod);
				}
			}
			u->RemoveUnitMod(mod);
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
	int musicBonusPhys = 0;
	int musicMultiplierPhys = 0;
	zoneMultiplierPhys = 0;
	vector<Modifier*> zMods;
	for (Unit* u : unitList) {
		vector<Modifier*> auras = u->GetAuras();
		for (Modifier* mod : auras) {
			zMods.push_back(mod);
			if (mod->GetModType() == MODIFIER_TYPE::MUSIC_AURA) {
				musicBonusPhys += mod->GetValue();
				musicMultiplierPhys += mod->GetMultiplier();
			}
			else {
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
		bool bMusical = false;
		
		for (Modifier* mod : u->GetModifiers()) {
			if (mod->GetName() == "Musical Modifier") {
				u->SetZoneBonusesPhys(zoneBonusPhys+musicBonusPhys, zoneMultiplierPhys+musicMultiplierPhys, zMods);
				bMusical = true;
			}
		}
		if (bMusical == false) {
			u->SetZoneBonusesPhys(zoneBonusPhys, zoneMultiplierPhys, zMods);
		}
	}
}

void UnitZone::EndTurnUpkeep(DataBase* database) {
	
	for (Unit* u : unitList) {

		for (Modifier* mod : u->GetModifiers()) {
			if (mod->GetModType() == MODIFIER_TYPE::UNIT_EOT_MOD) {
				u->AddModifier(mod->GetModifier());
			}
			if (mod->GetName() == "Stamina Reduced") {
				if (GetType() == ZONE_TYPE::Z_ATTACK) {
					mod->ApplyEOT();
				}
			}
			else {
				mod->ApplyEOT();
			}
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
	UpdatePositions();
}


void UnitZone::NewTurnUpkeep(DataBase* database) {
	for (Unit* u : unitList) {
		for (Modifier* mod : u->GetModifiers()) {
			if (mod->GetName() == "Stamina Reduced") {
				if (GetType() == ZONE_TYPE::Z_BLOCK) {
					mod->ApplyEOT();
				}
			}
			if (mod->GetModType() == MODIFIER_TYPE::SOT_EFFECT) {
				u->GetCard()->DoEffect(u,this);
			}
		}
		if (type == ZONE_TYPE::Z_BLOCK) {
			u->UpdateStats();
		}
	}
	
	CheckStamina();
	UpdatePositions();
}

void UnitZone::CheckStamina() {

	for (int i = 0; i < unitList.size();i++) {
		if (unitList[i]->GetStamina() <= 0) {
			if (unitList[i]->GetRetreated()) {
				RemoveUnit(unitList[i]);
				i--;
			}
			else {
				unitList[i]->Retreat();
			}
		}
	}
	
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

void UnitZone::Update(Time t) {
	pulseTimer += t.asSeconds();
	shaderPulse.setUniform("time", pulseTimer);
	for (Unit* u :GetUnits()) {
		u->Update(t);
	}
	CheckStamina();
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