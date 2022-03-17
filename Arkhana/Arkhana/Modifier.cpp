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
	duration = data.duration;
	bDynamic = data.bDynamic;
	effects = data.effect;
	texIcon.loadFromFile(data.filePath);
	icon.setTexture(texIcon);
	icon.setScale(0.75, 0.75);
	font.loadFromFile("Fonts/ManaSpace/manaspc.ttf");
	textDuration.setFont(font);
	textDuration.setCharacterSize(12);
	textDuration.setFillColor(Color::Blue);
	textDuration.setOutlineColor(Color::Black);
	textDuration.setString(to_string(duration));
}