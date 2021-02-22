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

	decklist.push_back(new Card(*database->CardList["Goblin"],database));
	decklist.push_back(new Card(*database->CardList["Rage"],database));

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

void Enemy::DrawActions() {

	for (int i = decklist.size() - 1; i >= 0; i--) {
		decklist[i]->Draw(window);
	}
	
}

void Enemy::NewTurnUpkeep() {
	ResetMana();
	bHasAttacked = false;
}

void Enemy::Update(Time t) {
	if (bAttacking) {
		if (attackTimer < attackDuration) {
			attackTimer += t.asSeconds();
			for (Unit* u : attackZone->GetUnits()) {
				float xdir = 0;
				float ydir = 0.1 * attackDirection;
				u->Move(Vector2f(xdir, ydir));
			}
		}
		else {
			attackTimer = 0;
			bAttacking = false;
		}
	}
	else {
		for (Card* c : decklist) {
			c->Update(t);
		}
		UpdateStrings();
	}
}

void Enemy::SetDetails(EncounterData* data, vector<Card*> startPlay, vector<Card*> dl) {

	health = data->health;
	armour = data->armour;
	maxMana = data->actionCount;
	startingPlay = startPlay;
	decklist = dl;
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