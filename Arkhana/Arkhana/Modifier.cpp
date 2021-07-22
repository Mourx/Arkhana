#include "Modifier.h"


Modifier::Modifier(ModifierData d) {
	data = d;
	GetData();
}

Modifier::Modifier(Modifier* m) {
	data = m->data;
	GetData();
}

void Modifier::GetData() {
	name = data.name;
	value = data.value;
	multiplier = data.multiplier;
	sType = data.sType;
	mType = data.mType;
	text = data.text;
	mText = data.mText;
	EOTChange = data.EOTChange;
	modifiers = data.modifiers;

	if (sType == STAT_TYPE::DMG_PHYSICAL) {
		texIcon.loadFromFile("Textures/Cards/sword.png");
	}
	else if (sType == STAT_TYPE::STAMINA) {
		texIcon.loadFromFile("Textures/Cards/flame.png");
	}
	icon.setTexture(texIcon);
}