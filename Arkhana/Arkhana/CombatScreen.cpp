#include "CombatScreen.h"


CombatScreen::CombatScreen(RenderWindow* w,DataBase* data,Player* p,Encounter* enc) {
	database = data;
	window = w;
	player = p;
	encounter = enc;
	player->Setup();

	enemy = encounter->GetEnemy();

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
	enemy->DrawBackground();
	player->DrawBackground();

	enemy->DrawForeground();
	player->DrawForeground();

	enemy->DrawActions();
	if (eNext != NULL) {
		eNext->Draw(window);
	}
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
			player->AnimateAttack();
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
	eNext = NULL;
	vector<Card*> list = enemy->GetDeckList();
	for (int i = list.size() - 1; i >= 0; i--) {
		FloatRect bounds = list[i]->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			eNext = list[i];
		}
	}
}

void CombatScreen::Update(Time t) {
	if (currentTurn == ENEMY) {
		
		if (enemy->HasAttacked() == false) {
			if (enemy->GetCurrentMana() >= 1) {
				Card* eCard = enemy->GetNext();
				if (eCard->IsAtTarget()) {
					// Maybe implement an AI controller? thinking how to emulate a player
					switch (eCard->GetType()) {
					case UNIT:
						eCard->Play(enemy->GetZones()[(int)ZONE_TYPE::Z_ATTACK]);
						break;
					case SPELL:
						eCard->Play(enemy->GetZones()[(int)ZONE_TYPE::Z_ATTACK]);
						break;
					}
					eCard->SetPosition(Vector2f(-200, -200));
					enemy->PlayNext();
				}
				else if(eCard->IsMoving() == false) {
					eCard->SetTarget(enemy->GetZones()[(int)ZONE_TYPE::Z_ATTACK]->GetIcon()->getPosition());
				}
			}
			else {
				enemy->AnimateAttack();
			}
		}
		else {
			if (enemy->GetAttacking() == false) {
				AdvanceTurn(PLAYER);
			}
		}
	}
	else if (currentTurn == PLAYER) {
		if (player->HasAttacked() == false) {
		}
		else {
			if (player->GetAttacking() == false) {
				AdvanceTurn(ENEMY);
			}
		}
	}

	player->Update(t);
	enemy->Update(t);

}

void CombatScreen::SetNextEnemyMove() {
	vector<Card*> cardList = enemy->GetDeckList();
	for (int i = 0; i < cardList.size();i++) {
		cardList[i]->SetPosition(eNextPos + Vector2f(0,-40*i));
	}
	
}

void CombatScreen::AdvanceTurn(COMBAT_TURN nextTurn) {

	CalculateCombat();
	CheckDeaths();
	switch (currentTurn) {
	case PLAYER:
		player->EndTurnUpkeep();
		enemy->NewTurnUpkeep();
		break;
	case ENEMY:
		enemy->EndTurnUpkeep();
		player->NewTurnUpkeep();
		break;
	}
	currentTurn = nextTurn;
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
		nextScreen = PATH_SCREEN;
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
	int playerPhysDamage = eAttack->GetCombinedPhysicalPower() - pBlock->GetCombinedPhysicalPower();

	if (playerPhysDamage <= 0) playerPhysDamage = 0;
	if (enemyPhysDamage <= 0) enemyPhysDamage = 0;

	player->DamagePhys(playerPhysDamage);
	enemy->DamagePhys(enemyPhysDamage);

}