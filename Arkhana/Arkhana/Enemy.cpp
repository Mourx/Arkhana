#include "Enemy.h"


Enemy::Enemy(RenderWindow* w,DataBase* data){
	database = data;
	window = w;

	attackZonePos = Vector2f(800, 180);
	blockZonePos = Vector2f(200, 180);

	attackZone = new UnitZone(0,this, Z_ENEMY,ZONE_TYPE::Z_ATTACK);
	attackZone->SetPosition(attackZonePos);

	blockZone = new UnitZone(1,this, Z_ENEMY,ZONE_TYPE::Z_BLOCK);
	blockZone->SetPosition(blockZonePos);

	zones.push_back(attackZone);
	zones.push_back(blockZone);

	physArmPos = Vector2f(1410, 445);
	healthPos = Vector2f(1410, 275);
	attackDirection = 1;
	
	txtPhysArmPos = Vector2f(1500, 465);
	txtHealthPos = Vector2f(1500, 295);

	cardList = database->CardListEnemy;

	decklist.push_back(new Card(*cardList["Goblin"],database));
	decklist.push_back(new Card(*cardList["Rage"],database));

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
	Card* c = decklist[cardIndex++];
	currentMana--;

	if (cardIndex >= decklist.size()) {
		cardIndex = 0;
		currentMana = 0;
	}
	return c;
}

void Enemy::DrawActions() {

	for (int i = decklist.size() - 1; i >= 0; i--) {
		decklist[i]->Draw(window);
	}
	
}

void Enemy::NewTurnUpkeep() {
	ResetMana();

	attackZone->NewTurnUpkeep(database);
	blockZone->NewTurnUpkeep(database);
	bHasAttacked = false;
}

void Enemy::EndTurnUpkeep() {
	SetNextMove();
	attackZone->EndTurnUpkeep(database);
	blockZone->EndTurnUpkeep(database);
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

void Enemy::SetDetails(EncounterData* data, vector<Card*> startPlay, vector<vector<Card*>> dl) {

	health = data->health;
	armour = data->armour;
	maxMana = data->actionCount;
	startingPlay = startPlay;
	decklists = dl;
	decklist = decklist;

	UpdateStrings();
}

void Enemy::SetNextMove() {
	decklist = decklists[rand() % decklists.size()];
	for (Card* c : decklist) {
		c->SetPosition(Vector2f(-200, -200));
		c->ResetTarget();
	}
	for (int i = 0; i < maxMana; i++) {
		int index = cardIndex + i;
		if (index >= decklist.size()) {
			index = 0;
		}
		decklist[index]->SetPosition(eNextPos + Vector2f(0, -40 * i));
	}
}