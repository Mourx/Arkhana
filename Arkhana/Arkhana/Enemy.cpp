#include "Enemy.h"


Enemy::Enemy(RenderTexture* w,DataBase* data){
	database = data;
	window = w;

	attackZonePos = Vector2f(000, 0);
	blockZonePos = Vector2f(600, 0);

	attackZone = new UnitZone(0,data->player,this, Z_ENEMY,ZONE_TYPE::Z_ATTACK);
	attackZone->SetPosition(attackZonePos);

	blockZone = new UnitZone(1, data->player,this, Z_ENEMY,ZONE_TYPE::Z_BLOCK);
	blockZone->SetPosition(blockZonePos);

	zones.push_back(attackZone);
	zones.push_back(blockZone);

	physArmPos = Vector2f(1230, 110);
	healthPos = Vector2f(1230, 10);
	attackDirection = 1;
	retreatDirection = -1;
	
	txtPhysArmPos = physArmPos + Vector2f(30, 75);
	txtHealthPos = healthPos + Vector2f(30, 75);

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
	physArmIcon.setScale(1.5, 1.5);




	texHealth.loadFromFile("Textures/GUI/health.png");
	healthIcon.setTexture(texHealth);
	healthIcon.setPosition(healthPos);
	healthIcon.setScale(1.5, 1.5);

	font.loadFromFile("Fonts/Arial/arial.ttf");

	txtHealth.setPosition(txtHealthPos);
	txtHealth.setFont(font);

	txtPhysArm.setPosition(txtPhysArmPos);
	txtPhysArm.setFont(font);



}

void Enemy::DrawBackground() {
	attackZone->Draw(window);
	blockZone->Draw(window);

	window->draw(deckIcon);
	window->draw(gemIcon);
	window->draw(discardIcon);
	window->draw(burntIcon);
	window->draw(healthIcon);
	window->draw(physArmIcon);

	
	window->draw(txtHealth);
	window->draw(txtPhysArm);
	window->draw(txtCurMana);
	window->draw(txtDeckSize);
	window->draw(txtDiscardSize);
	window->draw(txtBurntSize);
	for (Card* c : hand) {
		c->Draw(window);
	}
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
	bHasRetreated = false;
	bRetreating = false;
}

void Enemy::EndTurnUpkeep() {
	SetNextMove();
	attackZone->EndTurnUpkeep(database);
	blockZone->EndTurnUpkeep(database);
}

void Enemy::Update(Time t) {
	attackZone->Update(t);
	blockZone->Update(t);
	if (bAttacking) {
		if (attackTimer == 0) {
			attackTimer += t.asSeconds();
			for (Unit* u : attackZone->GetUnits()) {
				if (u->GetPPower() >= 1) {
					u->Attack();
				}
			}
		}
		else {
			attackTimer += t.asSeconds();
			if (attackTimer >= attackDuration) {
				bAttacking = false;
			}
		}
	}
	else if (bRetreating) {
		if (retreatTimer == 0) {
			retreatTimer += t.asSeconds();
			for (Unit* u : attackZone->GetUnits()) {
				if (u->GetStamina() == 1) {
					u->Retreat();
				}
			}
			/*
			for (Unit* u : blockZone->GetUnits()) {
				if (u->GetStamina() == 1) {
					u->Retreat();
				}
			}
			*/
		}
		else {
			retreatTimer += t.asSeconds();
			if (retreatTimer >= retreatDuration) {
				bRetreating = false;
			}
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
	if (database->bDebugMode) {
		maxHealth = 1;
		armour = 1;
	}
	else {
		maxHealth = data->health;
		armour = data->armour;
	}
	health = maxHealth;
	maxMana = data->actionCount;
	startingPlay = startPlay;
	decklists = dl;
	level = data->level;
	decklist = decklist;

	UpdateStrings();
}

void Enemy::SetNextMove() {
	decklist = decklists[rand() % decklists.size()];
	for (Card* c : decklist) {
		c->SetPosition(Vector2f(-200, -200));
		c->ResetTarget();
	}
	maxMana = decklist.size();
	for (int i = 0; i < decklist.size(); i++) {
		int index = cardIndex + i;
		if (index >= decklist.size()) {
			index = 0;
		}
		decklist[index]->SetPosition(eNextPos + Vector2f(0, -40 * i));
	}
}