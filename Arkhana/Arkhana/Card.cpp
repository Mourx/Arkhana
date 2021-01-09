#include "Card.h"


Card::Card(int ID) {
	if (ID == 0) {
		texIcon.loadFromFile("Textures/Cards/Magician/2_Thought.png");
	}
	else {
		texIcon.loadFromFile("Textures/Cards/Magician/1_Goblin.png");
	}
	icon.setTexture(texIcon);

	SetPosition(Vector2f(300, 300));
}