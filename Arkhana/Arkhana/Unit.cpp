#include "Unit.h"

using namespace std;
Unit::Unit(UnitData data,vector<Modifier*> mods) {
	basePhys = data.physPower;
	baseStamina = data.stamina;
	name = data.name;
	texIcon.loadFromFile(data.filePath);
	for (Modifier* mod : mods) {
		switch (mod->GetModType()) {
		case MODIFIER_TYPE::UNIT_MOD:
			unitMods.push_back(mod);
			break;
		case MODIFIER_TYPE::AURA_MOD:
			auraMods.push_back(mod);
			break;
		}
		
	}
	icon.setTexture(texIcon);
	
	texBackground.loadFromFile("Textures/GUI/unitBackground.png");
	unitBackground.setTexture(texBackground);

	font.loadFromFile("Fonts/Arial/arial.ttf");
	txtPhys.setFont(font);
	txtMag.setFont(font);
	

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
		ModifyStat(mod->GetStat(), mod->GetValue(), mod->GetMultiplier());
	}
	physicalPower += zoneBonusPhys;
	physicalPower += zoneMultiplierPhys;

	stamina += zoneBonusMag;
	stamina += zoneMultiplierMag;
	UpdateStrings();
}

void Unit::AddModifier(Modifier* mod) {
	unitMods.push_back(mod);
	UpdateStats();
}

void Unit::SetPosition(Vector2f p) {
	pos = p;
	GameObject::SetPosition(pos);
	unitBackground.setPosition(pos);
	txtPhys.setPosition(pos + txtPhysPos);
	txtMag.setPosition(pos + txtMagPos);
}

void Unit::Draw(RenderWindow* w) {
	
	w->draw(unitBackground);
	GameObject::Draw(w);
	w->draw(txtPhys);
	w->draw(txtMag);
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
		stamina += value;
		if (multiplier != 0) {
			stamina *= multiplier;
		}
		break;
	default:
		break;
	}
	

}

void Unit::Move(Vector2f offset) {
	SetPosition(icon.getPosition() + offset);
}