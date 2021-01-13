#include "Modifier.h"


Modifier::Modifier(ModifierData data) {
	name = data.name;
	value = data.value;
	multiplier = data.multiplier;
	sType = data.sType;
	mType = data.mType;
	text = data.text;
}