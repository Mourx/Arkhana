#include "Player.h"

Player::Player() {

}

Player::Player(RenderWindow* w, DataBase* data) {
	database = data;
	window = w;
	
	cardList = database->CardListRed;

	decklist.push_back(new Card(*cardList["Frog"],database));
	decklist.push_back(new Card(*cardList["Frog"], database));
	decklist.push_back(new Card(*cardList["Big Armour"], database));
	decklist.push_back(new Card(*cardList["Sudden Rot"], database));
	decklist.push_back(new Card(*cardList["Jelly"], database));
	decklist.push_back(new Card(*cardList["Big Armour"], database));
	decklist.push_back(new Card(*cardList["Big Armour"], database));
	decklist.push_back(new Card(*cardList["Big Armour"], database));

	// load these based on the selected Arcana
	armour = 10;
	health = 50;

	attackZone = new UnitZone(0,this,Z_PLAYER,ZONE_TYPE::Z_ATTACK);
	attackZone->SetPosition(attackZonePos);

	blockZone = new UnitZone(1,this,Z_PLAYER,ZONE_TYPE::Z_BLOCK);
	blockZone->SetPosition(blockZonePos);

	zones.push_back(attackZone);
	zones.push_back(blockZone);

	InitSprites();
	UpdateStrings();
}

void Player::InitSprites() {
	texPlayerBar.loadFromFile("Textures/GUI/playerBar.png");
	playerBarIcon.setTexture(texPlayerBar);
	playerBarIcon.setPosition(playerBarPos);

	texDeck.loadFromFile("Textures/GUI/deck_book.png");
	deckIcon.setTexture(texDeck);
	deckIcon.setPosition(deckPos);
	deckIcon.setScale(2, 2);

	texGem.loadFromFile("Textures/Cards/gem.png");
	gemIcon.setTexture(texGem);
	gemIcon.setPosition(gemPos);
	gemIcon.setScale(6, 6);

	texDiscard.loadFromFile("Textures/GUI/discard_bin.png");
	discardIcon.setTexture(texDiscard);
	discardIcon.setPosition(discardPos);
	discardIcon.setScale(2, 2);

	texBurnt.loadFromFile("Textures/GUI/burnt_void.png");
	burntIcon.setTexture(texBurnt);
	burntIcon.setPosition(burntPos);
	burntIcon.setScale(2, 2);


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

	txtCurMana.setFont(font);
	txtCurMana.setCharacterSize(60);
	txtCurMana.setFillColor(Color::Cyan);

	txtDeckSize.setFont(font);
	txtDiscardSize.setPosition(txtDiscardSizePos);
	txtBurntSize.setPosition(txtBurntSizePos);

	txtDiscardSize.setFont(font);
	txtBurntSize.setFont(font);
}

void Player::UpdateStrings() {
	txtHealth.setString(to_string(health));
	txtPhysArm.setString(to_string(armour));
	txtCurMana.setString(to_string(currentMana));
	txtDeckSize.setString(to_string(deck.size()));
	txtDiscardSize.setString(to_string(discard.size()));
	txtBurntSize.setString(to_string(burnt.size()));

	FloatRect tR = txtCurMana.getLocalBounds();
	txtCurMana.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtCurMana.setPosition(txtCurManaPos);

	tR = txtDeckSize.getLocalBounds();
	txtDeckSize.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtDeckSize.setPosition(txtDeckSizePos);

	tR = txtDiscardSize.getLocalBounds();
	txtDiscardSize.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtDiscardSize.setPosition(txtDiscardSizePos);

	tR = txtBurntSize.getLocalBounds();
	txtBurntSize.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtBurntSize.setPosition(txtBurntSizePos);
}

void Player::Update(Time t) {
	if (bAttacking) {
		if (attackTimer < attackDuration) {
			attackTimer += t.asSeconds();
			for (Unit* u : attackZone->GetUnits()) {
				float xdir = 0;
				float ydir = ((250*attackDirection) / attackDuration) * t.asSeconds();
				u->Move(Vector2f(xdir, ydir));
			}
		}
		else {
			attackTimer = 0;
			bAttacking = false;
		}
	}
	else {
		UpdateStrings();
	}
}
void Player::DrawPlayerBar() {
	window->draw(playerBarIcon);
}

void Player::DrawBackground() {
	attackZone->Draw(window);
	blockZone->Draw(window);
	
	window->draw(deckIcon);
	window->draw(gemIcon);
	window->draw(discardIcon);
	window->draw(burntIcon);
	window->draw(physArmIcon);
	window->draw(healthIcon);
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
	attackZone->NewTurnUpkeep(database);
	blockZone->NewTurnUpkeep(database);
	blockZone->CheckStamina();
}

void Player::EndTurnUpkeep() {

	attackZone->EndTurnUpkeep(database);
	blockZone->EndTurnUpkeep(database);
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
	UpdateStrings();
	SetCardPositions();
}

void Player::DiscardHand() {
	for (Card* c : hand) {
		discard.push_back(c);
	}
	hand.clear();
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