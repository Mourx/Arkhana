#include "Player.h"
#include "Faction.h"
Player::Player() {

}

Player::Player(RenderTexture* w, DataBase* data) {
	database = data;
	window = w;
	cardList = database->CardListRedUnlocked;
	

	attackZone = new UnitZone(0,this,data->enemy,Z_PLAYER,ZONE_TYPE::Z_ATTACK);
	attackZone->SetPosition(attackZonePos);

	blockZone = new UnitZone(1,this,data->enemy,Z_PLAYER,ZONE_TYPE::Z_BLOCK);
	blockZone->SetPosition(blockZonePos);

	zones.push_back(attackZone);
	zones.push_back(blockZone);

	shaderMana.loadFromFile("Textures/Shaders/manaPulse.vert", Shader::Vertex);

	InitSprites();
	UpdateStrings();
}

void Player::InitSprites() {
	texPlayerBar.loadFromFile("Textures/GUI/playerBar.png");
	playerBarIcon.setTexture(texPlayerBar);
	playerBarIcon.setPosition(playerBarPos);

	texPlayerGold.loadFromFile("Textures/GUI/gold.png");
	playerGoldIcon.setTexture(texPlayerGold);
	playerGoldIcon.setPosition(playerGoldPos);

	texDeck.loadFromFile("Textures/GUI/reserveTent.png");
	deckIcon.setTexture(texDeck);
	deckIcon.setPosition(deckPos);
	deckIcon.setScale(1, 1);

	deckSizeIcon.setTexture(texDeck);
	deckSizeIcon.setPosition(deckSizePos);
	deckSizeIcon.setScale(1, 1);


	texGem.loadFromFile("Textures/Cards/gem.png");
	gemIcon.setTexture(texGem);
	gemIcon.setPosition(gemPos);
	gemIcon.setScale(2, 2);

	texDiscard.loadFromFile("Textures/GUI/restTent.png");
	discardIcon.setTexture(texDiscard);
	discardIcon.setPosition(discardPos);
	discardIcon.setScale(1, 1);
	

	texBurnt.loadFromFile("Textures/GUI/burnt_void.png");
	burntIcon.setTexture(texBurnt);
	burntIcon.setPosition(burntPos);
	burntIcon.setScale(2, 2);


	texPhysArm.loadFromFile("Textures/GUI/armourPhysical.png");
	physArmIcon.setTexture(texPhysArm);
	physArmIcon.setPosition(physArmPos);
	physArmIcon.setScale(1.5, 1.5);

	
	texHealth.loadFromFile("Textures/GUI/health.png");
	healthIcon.setTexture(texHealth);
	healthIcon.setPosition(healthPos);
	healthIcon.setScale(1.5, 1.5);

	font.loadFromFile("Fonts/Arial/arial.ttf");
	coolFont.loadFromFile("Fonts/ManaSpace/manaspc.ttf");
	txtPlayerGold.setPosition(txtPlayerGoldPos);
	txtPlayerGold.setFont(coolFont);

	txtHealth.setPosition(txtHealthPos);
	txtHealth.setFont(coolFont);
	
	txtPhysArm.setPosition(txtPhysArmPos);
	txtPhysArm.setFont(coolFont);

	txtCurMana.setFont(coolFont);
	txtCurMana.setCharacterSize(30);
	txtCurMana.setFillColor(Color::Cyan);

	txtDeckTotalSize.setFont(coolFont);
	txtDeckTotalSize.setPosition(txtDeckTotalSizePos);

	txtDeckSize.setFont(coolFont);
	txtDiscardSize.setPosition(txtDiscardSizePos);
	txtBurntSize.setPosition(txtBurntSizePos);

	txtDiscardSize.setFont(coolFont);
	txtBurntSize.setFont(coolFont);
}

void Player::UpdateStrings() {
	txtHealth.setString(to_string(health) + "/" + to_string(maxHealth));
	txtDeckTotalSize.setString(to_string(decklist.size()));
	txtPhysArm.setString(to_string(armour));
	txtCurMana.setString(to_string(currentMana));
	txtDeckSize.setString(to_string(deck.size()));
	txtDiscardSize.setString(to_string(discard.size()));
	txtBurntSize.setString(to_string(burnt.size()));
	txtPlayerGold.setString(to_string(currentGold));

	FloatRect tR = txtCurMana.getLocalBounds();
	txtCurMana.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtCurMana.setPosition(txtCurManaPos);

	tR = txtDeckSize.getLocalBounds();
	txtDeckSize.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtDeckSize.setPosition(txtDeckSizePos);

	tR = txtDeckTotalSize.getLocalBounds();
	txtDeckTotalSize.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtDeckTotalSize.setPosition(txtDeckTotalSizePos);

	tR = txtDiscardSize.getLocalBounds();
	txtDiscardSize.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtDiscardSize.setPosition(txtDiscardSizePos);

	tR = txtBurntSize.getLocalBounds();
	txtBurntSize.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtBurntSize.setPosition(txtBurntSizePos);

	tR = txtHealth.getLocalBounds();
	txtHealth.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtHealth.setPosition(txtHealthPos);

	tR = txtPhysArm.getLocalBounds();
	txtPhysArm.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtPhysArm.setPosition(txtPhysArmPos);
}

void Player::SetFaction(Faction* f) {
	decklist.clear();
	faction = f;
	cardList = faction->GetCardList();
	decklist = faction->GetDecklist();
	if (database->bDebugMode) {
		maxHealth = 1;
		armour = 1;
	}

	// load these based on the selected Arcana
	armour = 10;
	health = maxHealth;
	
	InitSprites();
	UpdateStrings();
}

void Player::Update(Time t) {
	manaPulseTimer += t.asSeconds();
	shaderMana.setUniform("time", manaPulseTimer);
	attackZone->Update(t);
	blockZone->Update(t);
	if (bAttacking) {
		if (attackTimer == 0) {
			attackTimer += t.asSeconds();
			for (Unit* u : attackZone->GetUnits()) {
				if (u->GetPPower() >= 1 && !(u->IsPassive() || u->IsUndercover())) {
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
			/*for (Unit* u : blockZone->GetUnits()) {
				if (u->GetStamina() == 1) {
					u->Retreat();
				}
			}*/
		}
		else {
			retreatTimer += t.asSeconds();
			if (retreatTimer >= retreatDuration) {
				bRetreating = false;
			}
		}
	}
	else {
		UpdateStrings();
	}
}
void Player::DrawPlayerBar() {
	playerBarWindow->draw(playerBarIcon);
	playerBarWindow->draw(playerGoldIcon);
	playerBarWindow->draw(txtPlayerGold);
	playerBarWindow->draw(healthIcon);
	playerBarWindow->draw(txtHealth);
	playerBarWindow->draw(deckSizeIcon);
	playerBarWindow->draw(txtDeckTotalSize);
}

void Player::DrawBackground() {
	attackZone->Draw(window);
	blockZone->Draw(window);
	
	window->draw(deckIcon);
	if (currentMana > 0) {
		window->draw(gemIcon,&shaderMana);
	}
	else {
		window->draw(gemIcon);
	}
	
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

void Player::DrawForeground() {
	attackZone->DrawUnits(window);
	blockZone->DrawUnits(window);
	
}

void Player::NewTurnUpkeep() {
	
	DrawCards(CARDS_PER_TURN);
	ResetMana();
	bHasAttacked = false;
	bHasRetreated = false;
	UpdateCosts();
	attackZone->NewTurnUpkeep(database);
	blockZone->NewTurnUpkeep(database);
}

void Player::EndTurnUpkeep() {
	for(Modifier* mod : mods){
		if (mod->GetModType() == MODIFIER_TYPE::PLAYER_EOT_MOD) {
			if (mod->GetStat() == STAT_TYPE::ARMOUR_PHYSICAL) {
				ModifyArmour(mod->GetEOT());
			}
		}
	}
	attackZone->EndTurnUpkeep(database);
	blockZone->EndTurnUpkeep(database);
	SwapUnits();
	DiscardHand();
	
}

void Player::DrawCards(int amount) {
	for (int i = 0; i < amount; i++) {
		if (deck.size() != 0) {
			hand.push_back(deck[0]);
			deck.erase(deck.begin());
		}
		else if (deck.size() == 0) {
			for (Card* c : discard) {
				deck.push_back(c);
			}
			discard.clear();
			random_shuffle(deck.begin(), deck.end());
			if (deck.size() >= 1) {
				hand.push_back(deck[0]);
				deck.erase(deck.begin());
			}
		}
	}
	SetCardPositions();
}

void Player::UseCard(Card* c) {
	currentMana -= c->GetCost();
	discard.push_back(c);
	hand.erase(remove(hand.begin(), hand.end(), c), hand.end());
	c->SetHandPos(offScreenPos);
	UpdateCosts();
	UpdateStrings();
	SetCardPositions();
}

void Player::SwapUnits() {
	vector<tuple<ZONE_TYPE,Unit*>> swappers;
	for (Unit* u : attackZone->GetUnits()) {
		for (Modifier* mod : u->GetModifiers()) {
			if (mod->GetModType() == MODIFIER_TYPE::SWAP_ZONE) {
				swappers.push_back(make_tuple(ZONE_TYPE::Z_ATTACK,u));
			}
		}
	}
	for (Unit* u : blockZone->GetUnits()) {
		for (Modifier* mod : u->GetModifiers()) {
			if (mod->GetModType() == MODIFIER_TYPE::SWAP_ZONE) {
				swappers.push_back(make_tuple(ZONE_TYPE::Z_BLOCK, u));
			}
		}
	}
	for (tuple<ZONE_TYPE,Unit*> tuple : swappers) {
		ZONE_TYPE start = get<0>(tuple);
		Unit* u = get<1>(tuple);
		if (start == ZONE_TYPE::Z_ATTACK) {
			attackZone->RemoveUnit(u);
			blockZone->AddUnit(u,database);
		}
		else {
			blockZone->RemoveUnit(u);
			attackZone->AddUnit(u, database);
		}
	}
}

void Player::UpdateCosts() {
	effectCostChange = 0;
	unitCostChange = 0;
	for (Modifier* mod : mods) {
		if (mod->GetModType() == MODIFIER_TYPE::PLAYER_MOD) {
			switch (mod->GetStat()) {
			case STAT_TYPE::EFFECT_COST:
				effectCostChange += mod->GetValue();
				break;
			case STAT_TYPE::UNIT_COST:
				unitCostChange += mod->GetValue();
				break;
			default:
				break;
			}
		}
	}
		
	for (Card* c : hand) {
		if (c->GetType() == CARD_TYPE::UNIT) {
			c->SetCostChange(unitCostChange);
		}
		else {
			c->SetCostChange(effectCostChange);
		}
		
	}
}

void Player::DiscardHand() {
	for (Card* c : hand) {
		discard.push_back(c);
	}
	hand.clear();
}

void Player::RemoveFromDeck(Card* c) {
	decklist.erase(remove(decklist.begin(), decklist.end(), c), decklist.end());
}

void Player::Discard(Card* c) {


}

void Player::Discard(int index) {
	if (hand.size() > index) {
		discard.push_back(hand[index]);
		hand.erase(hand.begin() + index);
	}
}

void Player::DrawInitialHand() {
	DrawCards(CARDS_START);
}

void Player::Setup() {
	ResetMana();
	deck.clear();
	hand.clear();
	discard.clear();
	burnt.clear();
	for (Card* c : decklist) {
		deck.push_back(c);
	}
	
	hand.clear();
	discard.clear();
	armour = maxArmour;
	attackZone->ClearUnits();
	blockZone->ClearUnits();
	shuffle(begin(deck), end(deck), random);
	DrawInitialHand();
	SetCardPositions();
	bHasAttacked = false;
}

void Player::SetCardPositions() {
	if (hand.size() == 1) {
		hand[0]->SetHandPos(Vector2f(cardsStart.x, cardsStart.y));
		hand[0]->SetPosition(hand[0]->GetHandPos());
	}
	else {
		float xDiff = cardsEnd.x - cardsStart.x;
		float cardsAmount = (float)hand.size();
		float xSpacePerCard = xDiff / (cardsAmount - 1);
		for (int i = 0; i < hand.size(); i++) {
			hand[i]->SetHandPos(Vector2f(cardsStart.x + xSpacePerCard * i, cardsStart.y));
			hand[i]->SetPosition(hand[i]->GetHandPos());

		}
	}
}

void Player::DamagePhys(int damage) {
	if (armour >= damage) {
		armour -= damage;
	}
	else {
		health -= (damage - armour);
		armour = 0;
	}
	if (health <= 0) {
		health = 0;
	}
}

void Player::AnimateAttack() {
	if (attackZone->GetUnits().size() != 0) {
		bAttacking = true;
		attackTimer = 0;
	}
	bHasAttacked = true;
}

void Player::AnimateRetreat() {
	bool bRetreatNeeded = false;
	for (Unit* u : attackZone->GetUnits()) {
		if (u->GetStamina() == 1) {
			bRetreatNeeded = true;
		}
	}
	for (Unit* u : blockZone->GetUnits()) {
		if (u->GetStamina() == 1) {
			bRetreatNeeded = true;
		}
	}
	if (bRetreatNeeded) {
		bRetreating = true;
		retreatTimer = 0;
	}
	bHasRetreated = true;
}

void Player::SetEnemy(Player* e) {
	zones[0]->SetEnemy(e);
	zones[1]->SetEnemy(e);
}