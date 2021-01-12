#include "Card.h"


Card::Card(int ID) {
	if (ID == 0) {
		texIcon.loadFromFile("Textures/Cards/Magician/2_Thought.png");
		type = SPELL;
		modifier = new Modifier(0);
	}
	else if(ID == 1) {
		texIcon.loadFromFile("Textures/Cards/Magician/1_Goblin.png");
		type = UNIT;
		unitID = 0;
	}
	else {
		unitID = 1;
		texIcon.loadFromFile("Textures/Cards/Magician/3_Shrine.png");
		type = UNIT;
	}
	icon.setTexture(texIcon);

	SetPosition(Vector2f(300, 300));
}

void Card::Play(UnitZone* zone) {
	switch (type) {
	case UNIT:
		zone->AddUnit(new Unit(unitID));
		break;
	case SPELL:
		ApplyModifier(zone);
		break;
	}
}

void Card::ApplyModifier(UnitZone* zone) {

	switch (mod) {
	case MODIFIER_TYPE::ZONE_MOD:
		zone->ModifyUnits(modifier);
		break;
	case MODIFIER_TYPE::UNIT_MOD:
		break;
	case MODIFIER_TYPE::INSTANT_MOD:
		break;
	}

}