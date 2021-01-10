#include "Unit.h"

using namespace std;
Unit::Unit() {
	texIcon.loadFromFile("Textures/Units/gnome.png");
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


	basePhys = 1;
	baseMag = 0;
	physicalPower = basePhys;
	magicPower = baseMag;

	UpdateStrings();
}

void Unit::UpdateStrings() {
	txtPhys.setString(to_string((int)physicalPower));
	txtMag.setString(to_string((int)magicPower));

	FloatRect tR = txtPhys.getLocalBounds();
	txtPhys.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtPhys.setPosition(pos + txtPhysPos);
	
	tR = txtMag.getLocalBounds();
	txtMag.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtMag.setPosition(pos + txtMagPos);
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
	case DMG_PHYSICAL:
		physicalPower += value;
		if (multiplier != 0) {
			physicalPower *= multiplier;
		}
		break;
	case DMG_MAGIC:
		magicPower += value;
		if (multiplier != 0) {
			magicPower *= multiplier;
		}
		break;
	default:
		break;
	}
	

}