#include "Enemy.h"


Enemy::Enemy(RenderWindow* w,DataBase* data){
	database = data;
	window = w;

	attackZonePos = Vector2f(800, 0);
	blockZonePos = Vector2f(200, 0);

	attackZone = new UnitZone(0,Z_ENEMY);
	attackZone->SetPosition(attackZonePos);

	blockZone = new UnitZone(1, Z_ENEMY);
	blockZone->SetPosition(blockZonePos);

	zones.push_back(attackZone);
	zones.push_back(blockZone);

	physArmPos = Vector2f(1410, 265);
	healthPos = Vector2f(1410, 95);
	attackDirection = 1;
	
	txtPhysArmPos = Vector2f(1500, 285);
	txtHealthPos = Vector2f(1500, 115);

	deck.push_back(new Card(*database->CardList["Goblin"],database));
	deck.push_back(new Card(*database->CardList["Rage"],database));

	InitSprites();
	UpdateStrings();
}


void Enemy::InitSprites() {
	texPhysArm.loadFromFile("Textures/GUI/armourPhysical.png");
	physArmIcon.setTexture(texPhysArm);
	physArmIcon.setPosition(physArmPos);
	physArmIcon.setScale(2, 2);




	texHealth.loadFromFile("Textures/GUI/health.png");
	healthIcon.setTexture(texHealth);
	healthIcon.setPosition(healthPos);
	healthIcon.setScale(2, 2);

	font.loadFromFile("Fonts/Arial/arial.ttf");

	txtHealth.setPosition(txtHealthPos);
	txtHealth.setFont(font);

	txtPhysArm.setPosition(txtPhysArmPos);
	txtPhysArm.setFont(font);



}

Card* Enemy::PlayNext() {
	Card* c = deck[cardIndex++];
	if (cardIndex >= deck.size()) {
		cardIndex = 0;
	}
	currentMana--;
	return c;
}

void Enemy::Draw() {
	attackZone->Draw(window);
	blockZone->Draw(window);
	window->draw(physArmIcon);

	window->draw(healthIcon);
	window->draw(txtHealth);
	window->draw(txtPhysArm);

	deck[cardIndex]->Draw(window);
}

void Enemy::NewTurnUpkeep() {
	ResetMana();
	bHasAttacked = false;
}

void Enemy::SetDetails(EncounterData* data, vector<Card*> startPlay, vector<Card*> decklist) {

	health = data->health;
	armour = data->armour;
	maxMana = data->actionCount;
	startingPlay = startPlay;
	deck = decklist;

	for (Card* c : startingPlay) {
		// Maybe implement an AI controller? thinking how to emulate a player
		switch (c->GetType()) {
		case UNIT:
			c->Play(this->GetZones()[(int)ZONE_TYPE::Z_ATTACK]);
			break;
		case SPELL:
			c->Play(this->GetZones()[(int)ZONE_TYPE::Z_ATTACK]);
			break;
		}
	}
	UpdateStrings();
}