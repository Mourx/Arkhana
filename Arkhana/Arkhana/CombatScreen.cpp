#include "CombatScreen.h"


CombatScreen::CombatScreen(RenderWindow* w,DataBase* data) {
	database = data;
	window = w;
	player = new Player(w,database);
	player->Setup();

	enemy = new Enemy(w,database);

	texBackground.loadFromFile("Textures/GUI/combatBackground.png");
	background.setTexture(texBackground);
	background.setPosition(0, 0);

	endTurn = new EndTurnButton();
	endTurn->SetPosition(endTurnPos);
	SetNextEnemyMove();
}

void CombatScreen::Draw() {
	window->draw(background);
	endTurn->Draw(window);
	enemy->Draw();
	player->Draw();
	
}

void CombatScreen::MouseClicked(Vector2f mousePos) {
	for (Card* c : player->GetHand()) {
		FloatRect bounds = c->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			player->selectedCard = c;
			iconOffsetX = mousePos.x - c->GetIcon()->getPosition().x;
			iconOffsetY = mousePos.y - c->GetIcon()->getPosition().y;
			iconSnapPos = c->GetHandPos();
		}
	}
}

void CombatScreen::MouseReleased(Vector2f mousePos) {
	if (currentTurn == PLAYER) {
		if (player->selectedCard != NULL) {
			if (selectedZone != NULL && selectedZone->GetOwner() == player->selectedCard->GetZones()) {
				if (player->GetCurrentMana() < player->selectedCard->GetCost()) {
					player->selectedCard->SetPosition(player->selectedCard->GetHandPos());
				}
				else {
					player->selectedCard->Play(selectedZone);

					player->UseCard(player->selectedCard);

				}
			}
			else {
				player->selectedCard->SetPosition(player->selectedCard->GetHandPos());
			}
			player->selectedCard = NULL;

		}
		//end turn button functionality
		FloatRect bounds = endTurn->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			AdvanceTurn();
		}
	}
}

void CombatScreen::MouseMoved(Vector2f mousePos) {
	if (!player->selectedCard == NULL) {
		player->selectedCard->SetPosition(Vector2f(mousePos.x - iconOffsetX, mousePos.y - iconOffsetY));
	}
	selectedZone = NULL;
	for (UnitZone* u : player->GetZones()) {
		FloatRect bounds = u->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			selectedZone = u;
		}
	}
	for (UnitZone* u : enemy->GetZones()) {
		FloatRect bounds = u->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			selectedZone = u;
		}
	}
	for (Card* c : player->GetHand()) {
		FloatRect bounds = c->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			c->SetHover(true);
		}
		else {
			c->SetHover(false);
		}
	}
}

void CombatScreen::Update(Time t) {
	if (currentTurn == ENEMY) {
		Card* eCard = enemy->PlayNext();
		// Maybe implement an AI controller? thinking how to emulate a player
		switch (eCard->GetType()) {
		case UNIT:
			eCard->Play(enemy->GetZones()[(int)ZONE_TYPE::Z_ATTACK]);
			break;
		case SPELL:
			eCard->Play(enemy->GetZones()[(int)ZONE_TYPE::Z_ATTACK]);
			break;
		}
		SetNextEnemyMove();
		currentTurn = PLAYER;
		player->NewTurnUpkeep();
	}
	player->Update(t);
	enemy->Update(t);

}

void CombatScreen::SetNextEnemyMove() {
	eNext = enemy->GetNext();
	eNext->SetPosition(eNextPos);
}

void CombatScreen::AdvanceTurn() {

	CalculateCombat();
	CheckDeaths();
	currentTurn = ENEMY;
}

void CombatScreen::CheckDeaths() {
	if(player->GetHealth()<= 0){
		result = LOSS;
	}
	else if (enemy->GetHealth() <= 0) {
		result = WIN;
	}

	switch (result) {
	case LOSS:
		nextScreen = GAME_OVER;
		break;
	case WIN:
		nextScreen = REWARD_SCREEN;
		break;
	case ONGOING:
		nextScreen = NONE;
		break;
	}
}

void CombatScreen::CalculateCombat() {
	vector<UnitZone*> eZones = enemy->GetZones();
	vector<UnitZone*> pZones = player->GetZones();

	UnitZone* eAttack = eZones[(int)ZONE_TYPE::Z_ATTACK];
	UnitZone* eBlock = eZones[(int)(ZONE_TYPE::Z_BLOCK)];

	UnitZone* pAttack = pZones[(int)ZONE_TYPE::Z_ATTACK];
	UnitZone* pBlock = pZones[(int)ZONE_TYPE::Z_BLOCK];
	int enemyPhysDamage = pAttack->GetCombinedPhysicalPower() - eBlock->GetCombinedPhysicalPower();
	int enemyMagDamage = pAttack->GetCombinedMagicPower() - eBlock->GetCombinedMagicPower();

	int playerPhysDamage = eAttack->GetCombinedPhysicalPower() - pBlock->GetCombinedPhysicalPower();
	int playerMagDamage = eAttack->GetCombinedMagicPower() - pBlock->GetCombinedMagicPower();

	if (playerPhysDamage <= 0) playerPhysDamage = 0;
	if (playerMagDamage <= 0) playerMagDamage = 0;

	if (enemyPhysDamage <= 0) enemyPhysDamage = 0;
	if (enemyMagDamage <= 0) enemyMagDamage = 0;

	player->DamagePhys(playerPhysDamage);
	player->DamageMag(playerMagDamage);

	enemy->DamagePhys(enemyPhysDamage);
	enemy->DamageMag(enemyMagDamage);
}