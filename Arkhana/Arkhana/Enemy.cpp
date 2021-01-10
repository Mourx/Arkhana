#include "Enemy.h"


Enemy::Enemy(RenderWindow* w){
	window = w;

	attackZonePos = Vector2f(800, 0);
	blockZonePos = Vector2f(200, 0);

	physicalArmour = 10;
	magicArmour = 0;
	health = 10;

	attackZone = new UnitZone(0,Z_ENEMY);
	attackZone->SetPosition(attackZonePos);

	blockZone = new UnitZone(1, Z_ENEMY);
	blockZone->SetPosition(blockZonePos);

	zones.push_back(attackZone);
	zones.push_back(blockZone);

	physArmPos = Vector2f(1410, 265);
	magArmPos = Vector2f(1410, 180);
	healthPos = Vector2f(1410, 95);

	
	txtPhysArmPos = Vector2f(1500, 285);
	txtMagArmPos = Vector2f(1500, 200);
	txtHealthPos = Vector2f(1500, 115);

	deck.push_back(new Card(1));
	deck.push_back(new Card(0));

	InitSprites();
	UpdateStrings();
}


void Enemy::InitSprites() {
	texPhysArm.loadFromFile("Textures/GUI/armourPhysical.png");
	physArmIcon.setTexture(texPhysArm);
	physArmIcon.setPosition(physArmPos);
	physArmIcon.setScale(2, 2);

	texMagArm.loadFromFile("Textures/GUI/armourMagic.png");
	magArmIcon.setTexture(texMagArm);
	magArmIcon.setPosition(magArmPos);
	magArmIcon.setScale(2, 2);

	texHealth.loadFromFile("Textures/GUI/health.png");
	healthIcon.setTexture(texHealth);
	healthIcon.setPosition(healthPos);
	healthIcon.setScale(2, 2);

	font.loadFromFile("Fonts/Arial/arial.ttf");

	txtHealth.setPosition(txtHealthPos);
	txtHealth.setFont(font);

	txtPhysArm.setPosition(txtPhysArmPos);
	txtPhysArm.setFont(font);

	txtMagArm.setPosition(txtMagArmPos);
	txtMagArm.setFont(font);

}

Card* Enemy::PlayNext() {
	Card* c = deck[cardIndex++];
	if (cardIndex >= deck.size()) {
		cardIndex = 0;
	}
	return c;
}

void Enemy::Draw() {
	attackZone->Draw(window);
	blockZone->Draw(window);
	window->draw(physArmIcon);
	window->draw(magArmIcon);
	window->draw(healthIcon);
	window->draw(txtHealth);
	window->draw(txtPhysArm);
	window->draw(txtMagArm);
	deck[cardIndex]->Draw(window);
}