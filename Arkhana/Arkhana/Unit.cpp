#include "Unit.h"
#include "Card.h"

using namespace std;
Unit::Unit(UnitData data,vector<Modifier*> mods,Card* c) {
	basePhys = data.physPower;
	baseStamina = data.stamina;
	name = data.name;
	texIcon.loadFromFile(data.filePath);
	card = c;
	for (Modifier* mod : mods) {
		switch (mod->GetModType()) {
		case MODIFIER_TYPE::UNIT_MOD:
			unitMods.push_back(new Modifier(mod));
			break;
		case MODIFIER_TYPE::UNIT_EOT_MOD:
			unitMods.push_back(new Modifier(mod));
			break;
		case MODIFIER_TYPE::BOSS_RESIST_MOD:
			bBoss = true;
			unitMods.push_back(new Modifier(mod));
			break;
		case MODIFIER_TYPE::AURA_MOD:
			auraMods.push_back(new Modifier(mod));
			break;
		case MODIFIER_TYPE::AURA_EOT_MOD:
			auraMods.push_back(new Modifier(mod));
			break;
		case MODIFIER_TYPE::PLAYER_EOT_MOD:
			unitMods.push_back(new Modifier(mod));
			break;
		case MODIFIER_TYPE::PLAYER_MOD:
			unitMods.push_back(new Modifier(mod));
			break;
		case MODIFIER_TYPE::SWAP_ZONE:
			unitMods.push_back(new Modifier(mod));
			break;
		case MODIFIER_TYPE::ZONE_MOD_ATTACK:
			unitMods.push_back(new Modifier(mod));
			break;
		}
		
	}
	icon.setTexture(texIcon);
	
	texBackground.loadFromFile("Textures/Units/shadow.png");
	unitBackground.setTexture(texBackground);
	texHighlight.loadFromFile("Textures/Units/highlight.png");
	highlightIcon.setTexture(texHighlight);

	font.loadFromFile("Fonts/Arial/arial.ttf");
	txtPhys.setFont(font);
	txtMag.setFont(font);
	
	icon.setScale(2, 2);
	unitBackground.setScale(2, 2);
	txtPhys.setCharacterSize(20);
	txtMag.setCharacterSize(20);

	txtPhys.setFillColor(Color::Red);
	txtMag.setFillColor(Color::Blue);


	
	physicalPower = basePhys;
	stamina = baseStamina;


	UpdateStrings();
}

void Unit::UpdateStrings() {
	txtPhys.setString(to_string((int)physicalPower));
	txtMag.setString(to_string((int)stamina));

	FloatRect tR = txtPhys.getLocalBounds();
	txtPhys.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtPhys.setPosition(pos + txtPhysPos);
	
	tR = txtMag.getLocalBounds();
	txtMag.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtMag.setPosition(pos + txtMagPos);
}

void Unit::UpdateStats() {
	physicalPower = basePhys;
	stamina = baseStamina;
	for (Modifier* mod : unitMods) {
		switch (mod->GetModType()) {
		case MODIFIER_TYPE::ZONE_MOD:
		case MODIFIER_TYPE::ZONE_MOD_ATTACK:
			break;
		default:
			ModifyStat(mod->GetStat(), mod->GetValue(), mod->GetMultiplier());
			break;
		}
		
	}
	physicalPower += zoneBonusPhys;
	physicalPower += zoneMultiplierPhys;

	stamina += zoneBonusMag;
	stamina += zoneMultiplierMag;
	UpdateStrings();
}

void Unit::AddModifier(Modifier* mod) {
	if (mod->GetModType() == MODIFIER_TYPE::BOSS_RESIST_MOD) {
		bBoss = true;
	}
	unitMods.push_back(mod);
	UpdateStats();
}

void Unit::RemoveUnitMod(Modifier* mod) {
	unitMods.erase(remove(unitMods.begin(), unitMods.end(), mod), unitMods.end());
}


Modifier* Unit::GetStaminaMod() {
	for (Modifier* mod : unitMods) {
		if (mod->GetName() == "Stamina Reduced") {
			return mod;
		}
	}
	return NULL;
}

void Unit::SetPosition(Vector2f p) {
	pos = p;
	GameObject::SetPosition(pos);
	unitBackground.setPosition(pos+Vector2f(0,60));
	highlightIcon.setPosition(pos);
	txtPhys.setPosition(pos + txtPhysPos);
	txtMag.setPosition(pos + txtMagPos);
}

void Unit::Draw(RenderTexture* w) {
	
	w->draw(unitBackground);
	GameObject::Draw(w);
	if (!card->IsUndercover()) {
		w->draw(txtPhys);
	}
	w->draw(txtMag);
	if (bHover) {
		w->draw(highlightIcon);
	}
}

bool Unit::IsUndercover() {
	return card->IsUndercover();
}

void Unit::ModifyStat(STAT_TYPE stat, int value, int multiplier) {
	switch (stat) {
	case STAT_TYPE::DMG_PHYSICAL:
		physicalPower += value;
		if (multiplier != 0) {
			physicalPower *= multiplier;
		}
		break;
	case STAT_TYPE::STAMINA:
		if (IsBoss()) {
			break;
		}
		else {
			stamina += value;
			if (multiplier != 0) {
				stamina *= multiplier;
			}
			break;
		}
	default:
		break;
	}
	

}

void Unit::Move(Vector2f offset) {
	SetPosition(icon.getPosition() + offset);
}