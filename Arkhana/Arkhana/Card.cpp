#include "Card.h"


Card::Card(int ID) {
	if (ID == 0) {
		texIcon.loadFromFile("Textures/Cards/Magician/2_Thought.png");
		type = SPELL;
		modifier = new Modifier();
	}
	else {
		texIcon.loadFromFile("Textures/Cards/Magician/1_Goblin.png");
		type = UNIT;
	}
	icon.setTexture(texIcon);

	SetPosition(Vector2f(300, 300));
}

void Card::Play(UnitZone* zone) {
	switch (type) {
	case UNIT:
		zone->AddUnit(new Unit());
		break;
	case SPELL:
		ApplyModifier(zone);
		break;
	}
}

void Card::ApplyModifier(UnitZone* zone) {

	switch (mod) {
	case ZONE_MOD:
		zone->ModifyUnits(modifier);
		break;
	case UNIT_MOD:
		break;
	case INSTANT_MOD:
		break;
	}

}