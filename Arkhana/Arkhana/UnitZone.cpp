#include "UnitZone.h"
#include "DataBase.h"
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
	for (Modifier* mod : zoneMods) {
		mod->Draw(w);
	}
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
		if (!u->IsUndercover() && !u->IsPassive()) {
			bool bEvasive = false;
			for (Modifier* mod : u->GetModifiers()) {
				if (mod->GetName() == "Evasive") {
					bEvasive = true;
				}
			}
			if (bEvasive == false) {
				power += u->GetPPower();
			}
		}
	}
	power += zonePower;
	return power;
}

int UnitZone::GetEvasiveDamage() {
	int power = 0;
	for (Unit* u : unitList) {
		if (!u->IsUndercover() && !u->IsPassive()) {
			bool bEvasive = false;
			for (Modifier* mod : u->GetModifiers()) {
				if (mod->GetName() == "Evasive") {
					bEvasive = true;
				}
			}
			if (bEvasive == true) {
				power += u->GetPPower();
			}
		}
	}
	power += zonePower;
	return power;
}



void UnitZone::RemoveMod(Modifier* mod) {
	zoneMods.erase(remove(zoneMods.begin(), zoneMods.end(), mod), zoneMods.end());
	UpdateStatMods();
	UpdatePositions();
}


void UnitZone::RemoveMod(string modName) {
	Modifier* toRemove = NULL;
	for (Modifier* mod : zoneMods) {
		if (mod->GetName() == modName) {
			toRemove = mod;
		}
	}
	zoneMods.erase(remove(zoneMods.begin(), zoneMods.end(), toRemove), zoneMods.end());
	UpdateStatMods();
	UpdatePositions();
}



void UnitZone::AddMod(Modifier* mod) {
	zoneMods.push_back(mod);
	UpdateStatMods();
	cout << "yeet" << endl;
	UpdatePositions();
}

void UnitZone::UpdatePositions() {
	int offset = 0;
	int modOffset = 300;
	int dir = 1;
	if (ownerType == ZONE_OWNER::Z_ENEMY) {
		offset = 220;
		modOffset = 0;
		dir = -1;
	}
	for (int i = 0; i < unitList.size(); i++) {
		unitList[i]->SetPosition(this->GetIcon()->getPosition() + Vector2f(10 + (i / 3) * 10 + (i % 5) * 90, 10 + ((i / 5) * 125) * dir + offset));
	}
	hoverIcon.setPosition(pos + hoverOffset);
	for (int i = 0; i < zoneMods.size(); i++) {
		zoneMods[i]->SetPosition(this->GetIcon()->getPosition() + Vector2f(10 + 35 * i, 10 + modOffset));
	}

}

void UnitZone::AddUnit(Unit* u) {
	int count = unitList.size();
	vector<Modifier*> mods = u->GetModifiers();
	if (ownerType == ENEMY) {
		u->SetDirections(1, -1);
	}
	if(!u->IsBoss() && u->GetStaminaMod()== NULL) u->AddModifier((new Modifier(*database->modList["eot_stamina"])));
	for (Unit* unit : GetUnits()) {
		for (Modifier* mod : unit->GetModifiers()) {
			if (mod->GetModType() == MODIFIER_TYPE::UNIT_ENTER_APPLY_MOD) {
				for (Modifier* eachMod : mod->GetModifier()) {
					u->AddModifier(eachMod);
				}
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
		case MODIFIER_TYPE::EXHAUST_EFFECT:
			u->GetCard()->DoEffect(u, this);
			break;
		default:
			break;
		}
	}
	if (u->GetName() == "Potion Frog") {
		RemoveMod("potion_frog");
	}
	unitList.erase(remove(unitList.begin(),unitList.end(),u),unitList.end());
	for (Unit* u : unitList) {
		for (Modifier* mod : u->GetModifiers()) {
			if (mod->GetModType() == MODIFIER_TYPE::UNIT_LEAVE_MOD) {
				for (Modifier* eachMod : mod->GetModifier()) {
					u->AddModifier(new Modifier(eachMod));
				}
			}
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

void UnitZone::UpdateStatMods() {
	zoneBonusPhys = 0;
	zonePower = 0;
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
		
	}
	
	for (Modifier* mod : zoneMods) {
		if (mod->GetStat() == STAT_TYPE::ZONE_POWER) {
			zonePower += mod->GetValue();
		}
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
	for (Unit* u : unitList) {
		bool bMusical = false;

		for (Modifier* mod : u->GetModifiers()) {
			if (mod->GetName() == "Musical Modifier") {
				u->SetZoneBonusesPhys(zoneBonusPhys + musicBonusPhys, zoneMultiplierPhys + musicMultiplierPhys, zMods);
				bMusical = true;
			}
		}
		if (bMusical == false) {
			u->SetZoneBonusesPhys(zoneBonusPhys, zoneMultiplierPhys, zMods);
		}
	}
}

void UnitZone::EndTurnUpkeep() {
	bool bPotioned = false; 
	vector<Unit*> unitsUpdate = unitList;
	for (Modifier* mod : zoneMods) {
		if (type == ZONE_TYPE::Z_ATTACK) {
			if (mod->GetName() == "Potion Frog") {
				bPotioned = true;
			}
			mod->ModifyDuration(-1);
		}
		if (mod->GetModType() == MODIFIER_TYPE::EOT_EFFECT) {
			(new Card(*database->CardListAll["Goblin"]))->DoEffect(NULL, this, database->effectList[mod->GetEffect()]);
			mod->ModifyDuration(-1);
		}
	}
	for (int i = 0; i < unitsUpdate.size();i++) {
		Unit* u = unitsUpdate[i];
		for (Modifier* mods : u->GetModifiers()) {
			if (mods->GetModType() == MODIFIER_TYPE::UNIT_EOT_MOD) {
				if (mods->GetModifier().size() != 0) {
					for (Modifier* mod :mods->GetModifier()) {
						u->AddModifier(mod);
					}
				}
				else if (mods->GetName() == "Stamina Reduced" && bPotioned == false) {
					if (GetType() == ZONE_TYPE::Z_ATTACK) {
						mods->ApplyEOT();
					}
				}else if (mods->GetName() == "Potion Frog Stamina Reduced") {
					if (GetType() == ZONE_TYPE::Z_ATTACK) {
						mods->ApplyEOT();
					}
				}
				else {
					mods->ApplyEOT();
				}
			}
			else if (mods->GetModType() == MODIFIER_TYPE::EOT_EFFECT) {
				if (mods->GetEffect() != "") {
					u->GetCard()->DoEffect(u, this,database->effectList[mods->GetEffect()]);
				}
				else {
					u->GetCard()->DoEffect(u, this);
				}
			}
			
		}
		
		for (Modifier* mod : u->GetAuras()) {
			if (mod->GetModType() == MODIFIER_TYPE::AURA_EOT_MOD) {
				for (Unit* un : unitsUpdate) {
					for (Modifier* eachMod : mod->GetModifier()) {
						un->AddModifier(eachMod);
					}
				}
			}
			
		}
		u->UpdateStats();
	}
	CheckStamina();
	UpdatePositions();
}

void UnitZone::NewTurnUpkeep() {
	bool bPotioned = false;
	if (type == ZONE_TYPE::Z_BLOCK) {
		for (Modifier* mod : zoneMods) {
			if (mod->GetName() == "Potion Frog") {
				bPotioned = true;
			}
			mod->ModifyDuration(-1);
		}
	}
	for (int i = 0; i < zoneMods.size(); i++) {
		if (zoneMods[i]->GetDuration() <= 0) {
			RemoveMod(zoneMods[i]);
			i--;
		}
	}
	for (Unit* u : unitList) {
		for (Modifier* mod : u->GetModifiers()) {
			if (mod->GetName() == "Stamina Reduced" && bPotioned == false) {
				if (GetType() == ZONE_TYPE::Z_BLOCK) {
					mod->ApplyEOT();
				}
			}
			else if (mod->GetName() == "Potion Frog Stamina Reduced") {
				if (GetType() == ZONE_TYPE::Z_BLOCK) {
					mod->ApplyEOT();
				}
			}
			if (mod->GetModType() == MODIFIER_TYPE::UNIT_SOT_MOD) {
				mod->ApplyEOT();
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
		for (Modifier* mod : u->GetModifiers()) {
			if (mod->IsDynamic()) {
				if (mod->GetName() == "Builder Frog Modifier") {
					int count = 0;
					for (Unit* logCheck : unitList) {
						if (logCheck->GetName() == "Log") {
							count++;
						}
					}
					mod->SetValue(count);
					u->UpdateStats();
					u->UpdateStrings();

				}
				if (mod->GetName() == "Cool Frog Modifier") {
					bool active = false;
					for (Unit* cuteCheck : unitList) {
						if (cuteCheck->GetName() == "Cute Frog") {
							active = true;
						}
					}
					for (Unit* cuteCheck : owner->GetZones()[(int)GetOppositeType()]->GetUnits()) {
						if (cuteCheck->GetName() == "Cute Frog") {
							active = true;
						}
					}
					mod->SetActive(active);
					u->UpdateStats();
					u->UpdateStrings();

				}
				if (mod->GetName() == "Cute Frog Modifier") {
					bool active = false;
					for (Unit* coolCheck : unitList) {
						if (coolCheck->GetName() == "Cool Frog") {
							active = true;
						}
					}
					for (Unit* coolCheck : owner->GetZones()[(int)GetOppositeType()]->GetUnits()) {
						if (coolCheck->GetName() == "Cool Frog") {
							active = true;
						}
					}
					mod->SetActive(active);
					u->UpdateStats();
					u->UpdateStrings();

				}
				if (mod->GetName() == "Tuxedo Frog Attack Modifier") {
					bool active = false;
					for (Unit* coolCheck : unitList) {
						if (coolCheck->GetName() == "Cool Frog") {
							active = true;
						}
					}
					for (Unit* coolCheck : owner->GetZones()[(int)GetOppositeType()]->GetUnits()) {
						if (coolCheck->GetName() == "Cool Frog") {
							active = true;
						}
					}
					mod->SetActive(active);
					u->UpdateStats();
					u->UpdateStrings();


				}
				if (mod->GetName() == "Tuxedo Frog Stamina Modifier") {
					bool active = false;
					for (Unit* cuteCheck : unitList) {
						if (cuteCheck->GetName() == "Cute Frog") {
							active = true;
						}
					}
					for (Unit* cuteCheck : owner->GetZones()[(int)GetOppositeType()]->GetUnits()) {
						if (cuteCheck->GetName() == "Cute Frog") {
							active = true;
						}
					}
					mod->SetActive(active);
					u->UpdateStats();
					u->UpdateStrings();

				}
				if (mod->GetName() == "Wolf Modifier") {
					int count = 0;
					for (Unit* wolfCheck : unitList) {
						if (wolfCheck->GetName() == "Wolf" || wolfCheck->GetName() == "Alpha Wolf") {
							count++;
						}
					}
					mod->SetValue(count);
					u->UpdateStats();
					u->UpdateStrings();
				}
				if (mod->GetName() == "Alpha Wolf Modifier") {
					int count = 0;
					for (Unit* wolfCheck : unitList) {
						if (wolfCheck->GetName() == "Wolf") {
							count+=2;
						}
					}
					mod->SetValue(count);
					u->UpdateStats();
					u->UpdateStrings();
				}
			}
		}
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