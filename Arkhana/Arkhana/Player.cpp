#include "Player.h"

Player::Player() {

}

Player::Player(RenderWindow* w, DataBase* data) {
	database = data;
	window = w;
	
	deck.push_back(new Card(*database->CardList["Goblin"],database));
	deck.push_back(new Card(*database->CardList["Goblin"], database));
	deck.push_back(new Card(*database->CardList["Ogre"], database));
	deck.push_back(new Card(*database->CardList["Rage"], database));
	deck.push_back(new Card(*database->CardList["Shrine"], database));

	// load these based on the selected Arcana
	physicalArmour = 10;
	magicArmour = 10;
	health = 50;

	attackZone = new UnitZone(0,Z_PLAYER);
	attackZone->SetPosition(attackZonePos);

	blockZone = new UnitZone(1,Z_PLAYER);
	blockZone->SetPosition(blockZonePos);

	zones.push_back(attackZone);
	zones.push_back(blockZone);

	InitSprites();
	UpdateStrings();
}

void Player::InitSprites() {
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

	texMagArm.loadFromFile("Textures/GUI/armourMagic.png");
	magArmIcon.setTexture(texMagArm);
	magArmIcon.setPosition(magArmPos);
	magArmIcon.setScale(2, 2);

	texHealth.loadFromFile("Textures/GUI/health.png");
	healthIcon.setTexture(texHealth);
	healthIcon.setPosition(healthPos);
	healthIcon.setScale(2, 2);

	font.loadFromFile("Fonts/Arial/arial.ttf");

	
	txtHealth.setPosition(txtHealthPos);
	txtHealth.setFont(font);
	
	txtPhysArm.setPosition(txtPhysArmPos);
	txtPhysArm.setFont(font);
	txtMagArm.setPosition(txtMagArmPos);
	txtMagArm.setFont(font);

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
	txtPhysArm.setString(to_string(physicalArmour));
	txtMagArm.setString(to_string(magicArmour));
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

	UpdateStrings();
}

void Player::Draw() {
	attackZone->Draw(window);
	blockZone->Draw(window);
	window->draw(deckIcon);
	window->draw(gemIcon);
	window->draw(discardIcon);
	window->draw(burntIcon);
	window->draw(physArmIcon);
	window->draw(magArmIcon);
	window->draw(healthIcon);
	window->draw(txtHealth);
	window->draw(txtPhysArm);
	window->draw(txtMagArm);
	window->draw(txtCurMana);
	window->draw(txtDeckSize);
	window->draw(txtDiscardSize);
	window->draw(txtBurntSize);
	for (Card* c : hand) {
		c->Draw(window);
	}
}

void Player::NewTurnUpkeep() {
	DrawCards(1);
	currentMana = maxMana;
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

void Player::Discard(Card* c) {


}

void Player::Discard(int index) {
	if (hand.size() > index) {
		discard.push_back(hand[index]);
		hand.erase(hand.begin() + index);
	}
}

void Player::DrawInitialHand() {
	DrawCards(CARDS_PER_TURN);
}

void Player::Setup() {
	for (Card* c : hand) {
		deck.push_back(c);
	}
	for (Card* c : discard) {
		deck.push_back(c);
	}
	hand.clear();
	discard.clear();
	shuffle(begin(deck), end(deck), random);
	DrawInitialHand();
	SetCardPositions();
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
	if (physicalArmour >= damage) {
		physicalArmour -= damage;
	}
	else {
		health -= (damage - physicalArmour);
		physicalArmour = 0;
	}
	if (health <= 0) {
		health = 0;
	}
}


void Player::DamageMag(int damage) {
	if (magicArmour >= damage) {
		magicArmour -= damage;
	}
	else {
		health -= (damage - magicArmour);
		magicArmour = 0;
	}
	if (health <= 0) {
		health = 0;
	}
}