#pragma once
#include "Card.h"
#include "UnitZone.h"-
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
	void Update(Time t);

	vector<Card*> GetHand() { return hand; }

	Card* selectedCard = NULL;
	void UseCard(Card* c);
	int GetCurrentMana() { return currentMana; }
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

	Vector2f cardsStart = Vector2f(200, 700);
	Vector2f cardsEnd = Vector2f(1250, 700);
	Vector2f offScreenPos = Vector2f(-200, -200);

	RenderWindow* window;

	int CARDS_PER_TURN = 5;

	int magicArmour = 0;
	int physicalArmour = 0;
	int health = 0;

	int maxMana = 0;
	int currentMana = 0;

	UnitZone* attackZone;
	UnitZone* blockZone;
};

