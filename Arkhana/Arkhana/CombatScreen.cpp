#include "CombatScreen.h"


CombatScreen::CombatScreen(RenderTexture* w,DataBase* data,Player* p,Encounter* enc) {
	database = data;
	window = w;
	player = p;
	encounter = enc;
	player->Setup();
	type = COMBAT_SCREEN;
	enemy = encounter->GetEnemy();


	AI = new AIController(enemy, player);

	for (Card* c : enemy->GetStartingPlay()) {
		c->SetTarget(AI->GetTarget(c));
		switch (c->GetType()) {
		case CREATE_UNIT:
			c->Play();
			break;
		case APPLY_ZONE_MOD:
			c->Play();
			break;
		}
	}

	texBackground.loadFromFile("Textures/GUI/combatBackground.png");
	background.setTexture(texBackground);
	background.setPosition(0, 0);

	endTurn = new EndTurnButton();
	endTurn->SetPosition(endTurnPos);
	SetNextEnemyMove();

	description = enc->GetCombatDescription();
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
		if (player->selectedCard != NULL){
			Card* selCard = player->selectedCard;
			if (selectedZone != NULL) {
				if (selCard->GetZoneType() == ZONE_TYPE::Z_ANY || (selCard->GetZoneType() == selectedZone->GetType())) {
					if (selCard->GetZoneOwner() == Z_EITHER || selectedZone->GetOwnerType() == selCard->GetZoneOwner()) {
						if (player->GetCurrentMana() < selCard->GetCost()) {
							selCard->SetPosition(player->selectedCard->GetHandPos());
						}
						else {
							selCard->Play(selectedZone);
							player->UseCard(selCard);

						}
					}
					else {
						selCard->SetPosition(player->selectedCard->GetHandPos());
					}
					player->selectedCard = NULL;
				}
				else {
					selCard->SetPosition(player->selectedCard->GetHandPos());
					player->selectedCard = NULL;
				}
			}
			else {
				selCard->SetPosition(player->selectedCard->GetHandPos());
				player->selectedCard = NULL;
			}
		}
		//end turn button functionality
		FloatRect bounds = endTurn->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			player->AnimateAttack();
		}
	}
	selUnit = hoverUnit;
}

void CombatScreen::MouseMoved(Vector2f mousePos) {
	if (!player->selectedCard == NULL) {
		player->selectedCard->SetPosition(Vector2f(mousePos.x - iconOffsetX, mousePos.y - iconOffsetY));
	}
	selectedZone = NULL;
	hoverUnit = NULL;
	hoverCard = NULL;
	for (UnitZone* u : player->GetZones()) {
		FloatRect bounds = u->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			selectedZone = u;
		}
		for (Unit* unit : u->GetUnits()) {
			bounds = unit->GetIcon()->getGlobalBounds();
			if (bounds.contains(mousePos)) {
				hoverUnit = unit;
			}
		}
	}
	for (UnitZone* u : enemy->GetZones()) {
		FloatRect bounds = u->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			selectedZone = u;
		}
		for (Unit* unit : u->GetUnits()) {
			bounds = unit->GetIcon()->getGlobalBounds();
			if (bounds.contains(mousePos)) {
				hoverUnit = unit;
			}
		}
	}
	for (Card* c : player->GetHand()) {
		FloatRect bounds = c->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			c->SetHover(true);
			hoverCard = c;
			
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
			hoverCard = eNext;
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
					eCard->Play();
					eCard->SetPosition(Vector2f(-200, -200));
					enemy->PlayNext();
				}
				else if(eCard->IsMoving() == false) {
					eCard->SetTarget(AI->GetTarget(eCard));
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
	enemy->SetNextMove();

	
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
	int playerPhysDamage = eAttack->GetCombinedPhysicalPower() - pBlock->GetCombinedPhysicalPower();

	if (playerPhysDamage <= 0) playerPhysDamage = 0;
	if (enemyPhysDamage <= 0) enemyPhysDamage = 0;

	if (currentTurn == PLAYER) {
		enemy->DamagePhys(enemyPhysDamage);
	}
	else {
		player->DamagePhys(playerPhysDamage);
	}
	

}

void CombatScreen::SetInfo(InfoPane* info) {
	info->SetDescription(description);
	info->SetScreenTitle("Combat!");
	if (selUnit != NULL) {
		info->SetUnitInfo(selUnit);
	}
	if (hoverCard != NULL) {
		info->SetCardInfo(hoverCard);
	}if (hoverUnit != NULL) {
		info->SetUnitInfo(hoverUnit);
	}
}