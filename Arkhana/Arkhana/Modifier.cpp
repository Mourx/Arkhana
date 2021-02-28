#include "Modifier.h"


Modifier::Modifier(ModifierData d) {
	data = d;
	name = data.name;
	value = data.value;
	multiplier = data.multiplier;
	sType = data.sType;
	mType = data.mType;
	text = data.text;
	EOTChange = data.EOTChange;
}

Modifier::Modifier(Modifier* m) {
	data = m->data;
	name = data.name;
	value = data.value;
	multiplier = data.multiplier;
	sType = data.sType;
	mType = data.mType;
	text = data.text;
	EOTChange = data.EOTChange;
}