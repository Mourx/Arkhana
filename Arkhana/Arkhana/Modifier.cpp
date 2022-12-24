#include "Modifier.h"
#include "DataBase.h"


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
	except = data.except;
	texIcon.loadFromFile(data.filePath);
	icon.setTexture(texIcon);
	icon.setScale(0.75, 0.75);
	textDuration.setFont(database->coolFont);
	textDuration.setCharacterSize(12);
	textDuration.setFillColor(Color::Blue);
	textDuration.setOutlineColor(Color::Black);
	textDuration.setString(to_string(duration));
}