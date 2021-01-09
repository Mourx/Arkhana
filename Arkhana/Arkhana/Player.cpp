#include "Player.h"


Player::Player(RenderWindow* w) {

	window = w;

	deck.push_back(new Card(0));
	deck.push_back(new Card(0));
	deck.push_back(new Card(1));
	deck.push_back(new Card(1));
	deck.push_back(new Card(1));
	deck.push_back(new Card(1));
	deck.push_back(new Card(1));
	deck.push_back(new Card(1));
	deck.push_back(new Card(1));
	deck.push_back(new Card(1));
}

void Player::DrawCards(int amount) {
	for (int i = 0; i < amount; i++) {
		if (deck.size() != 0) {
			hand.push_back(deck[0]);
			deck.erase(deck.begin());
		}
	}
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
	float xDiff = cardsEnd.x - cardsStart.x;
	float cardsAmount = (float)hand.size();
	float xSpacePerCard = xDiff / (cardsAmount-1);
	for (int i = 0; i < hand.size(); i++) {
		hand[i]->SetPosition(Vector2f(cardsStart.x + xSpacePerCard * i,cardsStart.y));
	}
}

void Player::Draw() {
	for (Card* c : hand) {
		c->Draw(window);
	}
}