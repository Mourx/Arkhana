#pragma once
#include "Card.h"
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

class Player
{
public:
	Player(RenderWindow* w);
	~Player() {}
	void Setup();
	void Draw();
protected:
	void DrawCards(int amount);
	void Discard(Card* card);
	void Discard(int index);
	void DrawInitialHand();
	void SetCardPositions();
	vector<Card*> deck;
	vector<Card*> hand;
	vector<Card*> discard;
	vector<Card*> burnt;
	random_device random;

	Vector2f cardsStart = Vector2f(100, 800);
	Vector2f cardsEnd = Vector2f(750, 800);

	RenderWindow* window;

	int CARDS_PER_TURN = 8;
};

