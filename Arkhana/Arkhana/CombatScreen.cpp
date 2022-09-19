#include "CombatScreen.h"


CombatScreen::CombatScreen(RenderTexture* w,DataBase* data,Player* p,Encounter* enc) {
	database = data;
	window = w;
	player = p;
	encounter = enc;
	player->Setup();
	type = COMBAT_SCREEN;
	enemy = encounter->GetEnemy();
	database->enemy = enemy;
	player->SetEnemy(enemy);
	AI = new AIController(enemy, player);

	for (Card* c : enemy->GetStartingPlay()) {
		c->SetTarget(AI->GetTarget(c));
		switch (c->GetType()) {
		case UNIT:
			c->Play(hoverUnit);
			break;
		default:
			c->Play(hoverUnit);
			break;
		}
	}

	texBackground.loadFromFile("Textures/GUI/combatBackground.png");
	background.setTexture(texBackground);
	background.setPosition(0, 0);

	texPDS.loadFromFile("Textures/GUI/damageBackground.png");
	playerDamageSign.setTexture(texPDS);
	enemyDamageSign.setTexture(texPDS);
	playerDamageSign.setPosition(pdsPos);
	enemyDamageSign.setPosition(edsPos);
	

	textPBlock.setFont(database->coolFont);
	textPAttack.setFont(database->coolFont);
	textEBlock.setFont(database->coolFont);
	textEAttack.setFont(database->coolFont);

	textEstDamagePlayer.setFont(database->coolFont);
	textEstDamageEnemy.setFont(database->coolFont);


	textPBlock.setCharacterSize(100);
	textPAttack.setCharacterSize(100);
	textPBlock.setOutlineColor(Color(0,0,0,100));
	textPAttack.setOutlineColor(Color(0,0,0,100));
	textPBlock.setOutlineThickness(2);
	textPAttack.setOutlineThickness(2);

	textEBlock.setCharacterSize(100);
	textEAttack.setCharacterSize(100);
	textEBlock.setOutlineColor(Color(0, 0, 0, 100));
	textEAttack.setOutlineColor(Color(0, 0, 0, 100));
	textEBlock.setOutlineThickness(2);
	textEAttack.setOutlineThickness(2);
	shaderTranslucent.loadFromFile("Textures/Shaders/translucent.vert", Shader::Vertex);
	
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
	window->draw(textPBlock);
	window->draw(textPAttack);
	window->draw(textEBlock);
	window->draw(textEAttack);
	window->draw(textEstDamagePlayer);
	window->draw(textEstDamageEnemy);
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
			if (selectedZone != NULL) {
				Card* selCard = player->selectedCard;
				if (player->GetCurrentMana() < selCard->GetCost() || (selCard->IsUnitTarget() && hoverUnit == NULL)) {
					selCard->SetPosition(player->selectedCard->GetHandPos());
					player->selectedCard = NULL;
				}
				else {
					selCard->Play(selectedZone, hoverUnit);
					player->UseCard(selCard);
					player->selectedCard = NULL;
				}
			}
			else {
				player->selectedCard->SetPosition(player->selectedCard->GetHandPos());
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
	if (player->selectedCard != NULL || hoverCard != NULL) {
		Card* card;
		if (player->selectedCard != NULL) {
			card = player->selectedCard;
			card->SetPosition(Vector2f(mousePos.x - iconOffsetX, mousePos.y - iconOffsetY));
		}
		else {
			
			card = hoverCard;
		}
		
		for (UnitZone* u : player->GetZones()) {
			u->SetTargetable(false);
			if (card->GetZoneType() == ZONE_TYPE::Z_ANY || (card->GetZoneType() == u->GetType())) {
				if (card->GetZoneOwner() == Z_EITHER || u->GetOwnerType() == card->GetZoneOwner()) {
					u->SetTargetable(true);
				}
			}
			else if (card->GetZoneType() == ZONE_TYPE::Z_MIN_3_UNIT) {
				if (u->GetUnits().size() >= 3) {
					if (card->GetZoneOwner() == Z_EITHER || u->GetOwnerType() == card->GetZoneOwner()) {
						u->SetTargetable(true);
					}
				}
			}
		}
		for (UnitZone* u : enemy->GetZones()) {
			u->SetTargetable(false);
			if (card->GetZoneType() == ZONE_TYPE::Z_ANY || (card->GetZoneType() == u->GetType())) {
				if (card->GetZoneOwner() == Z_EITHER || u->GetOwnerType() == card->GetZoneOwner()) {
					u->SetTargetable(true);
				}
			}
			else if (card->GetZoneType() == ZONE_TYPE::Z_MIN_3_UNIT) {
				if (u->GetUnits().size() >= 3) {
					if (card->GetZoneOwner() == Z_EITHER || u->GetOwnerType() == card->GetZoneOwner()) {
						u->SetTargetable(true);
					}
				}
			}
		}
	}
	else {
		for (UnitZone* u : player->GetZones()) {
			u->SetTargetable(false);
		}
		for (UnitZone* u : enemy->GetZones()) {
			u->SetTargetable(false);
		}
	}
	
	selectedZone = NULL;
	hoverUnit = NULL;
	hoverCard = NULL;
	for (UnitZone* u : player->GetZones()) {
		u->SetHover(false);
		FloatRect bounds = u->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			selectedZone = u;
		}
		for (Unit* unit : u->GetUnits()) {
			bounds = unit->GetIcon()->getGlobalBounds();
			if (bounds.contains(mousePos)) {
				hoverUnit = unit;
				unit->SetHover(true);
			}
			else {
				unit->SetHover(false);
			}
		}
	}
	for (UnitZone* u : enemy->GetZones()) {
		u->SetHover(false);
		FloatRect bounds = u->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			selectedZone = u;
		}
		for (Unit* unit : u->GetUnits()) {
			bounds = unit->GetIcon()->getGlobalBounds();
			if (bounds.contains(mousePos)) {
				hoverUnit = unit;
				unit->SetHover(true);
			}
			else {
				unit->SetHover(false);
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
	if (!player->selectedCard == NULL && selectedZone != NULL) {
		if (!selectedZone->IsTargetable()){
			selectedZone = NULL;
		}		
	}
	vector<Card*> list = enemy->GetDeckList();
	for (int i = list.size() - 1; i >= 0; i--) {
		FloatRect bounds = list[i]->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			eNext = list[i];
			hoverCard = eNext;
		}
	}
	FloatRect bounds = endTurn->GetIcon()->getGlobalBounds();
	if (bounds.contains(mousePos)) {
		endTurn->SetHover(true);
	}
	else {
		endTurn->SetHover(false);
	}
}

void CombatScreen::Update(Time t) {
	UpdateDamagePredictions();

	if (currentTurn == ENEMY) {
		bool bWaitRetreats = false;
		for (Unit* u : enemy->GetZones()[(int)ZONE_TYPE::Z_BLOCK]->GetUnits()) {
			if (u->IsRetreating()) {
				bWaitRetreats = true;
			}
		}
		if (!bWaitRetreats) {
			if (enemy->HasAttacked() == false) {
				if (enemy->GetCurrentMana() >= 1) {
					Card* eCard = enemy->GetNext();
					if (eCard->IsAtTarget()) {
						// Maybe implement an AI controller? thinking how to emulate a player
						eCard->Play(hoverUnit);
						eCard->SetPosition(Vector2f(-200, -200));
						enemy->PlayNext();
					}
					else if (eCard->IsMoving() == false) {
						eCard->SetTarget(AI->GetTarget(eCard));
					}
				}
				else {
					enemy->AnimateAttack();
				}
			}
			else {
				if (enemy->GetAttacking() == false && !enemy->HasRetreated()) {
					enemy->GetZones()[(int)ZONE_TYPE::Z_ATTACK]->UpdatePositions();
					enemy->AnimateRetreat();
				}
				else if (enemy->HasRetreated() && enemy->GetRetreating() == false) {
					AdvanceTurn(PLAYER);
				}
			}
		}
	}
	else if (currentTurn == PLAYER) {
		if (player->HasAttacked() == false) {
		}
		else {
			if (player->GetAttacking() == false && !player->HasRetreated()) {
				player->GetZones()[(int)ZONE_TYPE::Z_ATTACK]->UpdatePositions();
				player->AnimateRetreat();
			}else if (player->HasRetreated() && player->GetRetreating() == false) {
				AdvanceTurn(ENEMY);
			}
		}
	}

	player->Update(t);
	enemy->Update(t);
	if (!player->GetRetreating()) {
		CheckDeaths();
	}
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
		player->EndTurnUpkeep();
		player->NewTurnUpkeep();
		player->ClearUnits();
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
		//enemy->DamagePhys(enemyPhysDamage);
	}
	else {
		//player->DamagePhys(playerPhysDamage);
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


void CombatScreen::UpdateDamagePredictions() {

	vector<UnitZone*> eZones = enemy->GetZones();
	vector<UnitZone*> pZones = player->GetZones();

	UnitZone* eAttack = eZones[(int)ZONE_TYPE::Z_ATTACK];
	UnitZone* eBlock = eZones[(int)(ZONE_TYPE::Z_BLOCK)];

	UnitZone* pAttack = pZones[(int)ZONE_TYPE::Z_ATTACK];
	UnitZone* pBlock = pZones[(int)ZONE_TYPE::Z_BLOCK];

	int enemyEvasiveDamage = pAttack->GetEvasiveDamage();
	int playerEvasiveDamage = eAttack->GetEvasiveDamage();

	int enemyPhysDamage = pAttack->GetCombinedPhysicalPower() - eBlock->GetCombinedPhysicalPower();
	int playerPhysDamage = eAttack->GetCombinedPhysicalPower() - pBlock->GetCombinedPhysicalPower();
	if (enemyPhysDamage <= 0) {
		enemyPhysDamage = 0;
	}
	if (playerPhysDamage <= 0) {
		playerPhysDamage = 0;
	}
	Color blue = Color(0, 0, 255, 100);
	Color red = Color(255, 0, 0, 100);

	textPBlock.setFillColor(blue);
	textEBlock.setFillColor(blue);
	textPAttack.setFillColor(red);
	textEAttack.setFillColor(red);


	textPBlock.setString(to_string(pBlock->GetCombinedPhysicalPower()));
	textPAttack.setString(to_string(pAttack->GetCombinedPhysicalPower()));
	textEBlock.setString(to_string(eBlock->GetCombinedPhysicalPower()));
	textEAttack.setString(to_string(eAttack->GetCombinedPhysicalPower()));

	FloatRect tR = textPAttack.getLocalBounds();
	textPAttack.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	textPAttack.setPosition(textPAttackPos);
	
	tR = textPBlock.getLocalBounds();
	textPBlock.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	textPBlock.setPosition(textPBlockPos);

	tR = textEBlock.getLocalBounds();
	textEBlock.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	textEBlock.setPosition(textEBlockPos);

	tR = textEAttack.getLocalBounds();
	textEAttack.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	textEAttack.setPosition(textEAttackPos);


	player->SetDamageDealt(enemyPhysDamage+enemyEvasiveDamage);
	enemy->SetDamageDealt(playerPhysDamage+playerEvasiveDamage);

	blue = Color(0, 0, 255, 190);
	red = Color(255, 0, 0, 190);

	textEstDamagePlayer.setFillColor(red);
	textEstDamageEnemy.setFillColor(red);

	textEstDamagePlayer.setString(to_string(playerPhysDamage + playerEvasiveDamage));
	tR = textEstDamagePlayer.getLocalBounds();
	textEstDamagePlayer.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	textEstDamagePlayer.setPosition(estDamagePlayerPos);
	
	textEstDamageEnemy.setString(to_string(enemyPhysDamage + enemyEvasiveDamage));
	tR = textEstDamageEnemy.getLocalBounds();
	textEstDamageEnemy.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	textEstDamageEnemy.setPosition(estDamageEnemyPos);
}