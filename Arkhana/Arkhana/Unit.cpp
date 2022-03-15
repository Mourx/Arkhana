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
		case MODIFIER_TYPE::MUSIC_AURA:
			auraMods.push_back(new Modifier(mod));
			break;
		default:
			unitMods.push_back(new Modifier(mod));
		}
		
	}
	icon.setTexture(texIcon);
	
	retreatPhrases.push_back("Run away!");
	retreatPhrases.push_back("Fleeeeee!");
	retreatPhrases.push_back("I'm Tired");
	retreatPhrases.push_back("Not Today");

	texBackground.loadFromFile("Textures/Units/shadow.png");
	unitBackground.setTexture(texBackground);
	texHighlight.loadFromFile("Textures/Units/highlight.png");
	highlightIcon.setTexture(texHighlight);
	texSpeech.loadFromFile("Textures/GUI/speechBubble.png");
	speechIcon.setTexture(texSpeech);

	font.loadFromFile("Fonts/Arial/arial.ttf");
	txtPhys.setFont(font);
	txtMag.setFont(font);
	txtSpeech.setFont(font);
	
	icon.setScale(2, 2);
	unitBackground.setScale(2, 2);
	txtPhys.setCharacterSize(20);
	txtMag.setCharacterSize(20);
	txtSpeech.setCharacterSize(14);

	txtPhys.setFillColor(Color::Red);
	txtMag.setFillColor(Color::Blue);
	txtSpeech.setFillColor(Color::Black);

	txtSpeech.setString("Run Away!");
	
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

	tR = txtSpeech.getLocalBounds();
	txtSpeech.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtSpeech.setPosition(speechIcon.getPosition() + txtSpeechPos);
}

void Unit::Update(Time t) {
	GameObject::Update(t);
	nudgeTimer += t.asSeconds();
	if (nudgeTimer >= nudgeDelay) {
		nudgeDir *= -1;
		icon.setPosition(pos + (Vector2f(0, 3 * nudgeDir)));

		nudgeTimer = 0;
	}
	if (bAttacking) {
		if (attackTimer < attackDuration) {
			attackTimer += t.asSeconds();
			int flipdir = attackTimer <= attackDuration / 2 ? 1 : -1;
			float xdir = 0;// ((150 * attackDirection) / attackDuration)* flipdir* t.asSeconds();
			float ydir = ((400* attackDirection)/attackDuration) * flipdir * t.asSeconds();
			Move(Vector2f(xdir, ydir));
		}
		else {
			attackTimer = 0;
			bAttacking = false;
		}
	}
	else if (bRetreating) {
		if (retreatTimer < retreatDuration) {
			retreatTimer += t.asSeconds();
			float xdir = 0;
			float ydir = ((250 * retreatDirection) / retreatDuration) * t.asSeconds();
			Move(Vector2f(xdir, ydir));				
		}
		else {
			retreatTimer = 0;
			bRetreating = false;
			bHasRetreated = true;
		}
	}

}

void Unit::UpdateStats() {
	physicalPower = basePhys;
	stamina = baseStamina;
	for (Modifier* mod : unitMods) {
		switch (mod->GetModType()) {
		case MODIFIER_TYPE::ZONE_MOD:
		case MODIFIER_TYPE::ZONE_MOD_ATTACK:
		case MODIFIER_TYPE::UNIT_ENTER_APPLY_MOD:
			break;
		default:
			if (mod->GetActive()) {
				ModifyStat(mod->GetStat(), mod->GetValue(), mod->GetMultiplier());
			}
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
	speechIcon.setPosition(pos + Vector2f(40, -20));
	txtPhys.setPosition(pos + txtPhysPos);
	txtMag.setPosition(pos + txtMagPos);
	txtSpeech.setPosition(speechIcon.getPosition() + txtSpeechPos);

}

void Unit::Draw(RenderTexture* w) {
	
	w->draw(unitBackground);
	GameObject::Draw(w);
	if (!card->IsUndercover() && !card->IsPassive()) {
		w->draw(txtPhys);
	}
	w->draw(txtMag);
	if (bHover) {
		w->draw(highlightIcon);
	}
	if (bRetreating) {
		w->draw(speechIcon);
		w->draw(txtSpeech);
	}
}

bool Unit::IsUndercover() {
	return card->IsUndercover();
}

bool Unit::IsPassive() {
	return card->IsPassive();
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


void Unit::Retreat() {
	if (!bRetreating) {
		txtSpeech.setString(retreatPhrases[rand() % retreatPhrases.size()]);
		UpdateStrings();
		bRetreating = true;
	}
}