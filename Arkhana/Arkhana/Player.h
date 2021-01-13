#pragma once
#include "Card.h"
#include "UnitZone.h"
#include <vector>
#include <algorithm>
#include <random>
#include "DataBase.h"

using namespace std;

class Player
{
public:
	Player();
	Player(RenderWindow* w,DataBase* data);
	~Player() {}
	void Setup();
	void Draw();
	void Update(Time t);

	vector<Card*> GetHand() { return hand; }

	Card* selectedCard = NULL;
	void UseCard(Card* c);
	int GetCurrentMana() { return currentMana; }
	vector<UnitZone*> GetZones() { return zones; }
	void DamagePhys(int damage);
	void DamageMag(int damage);
protected:
	void DrawCards(int amount);
	void Discard(Card* card);
	void Discard(int index);
	void DrawInitialHand();
	void SetCardPositions();
	virtual void InitSprites();
	virtual void UpdateStrings();
	DataBase* database;
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

	int magicArmour = 30;
	int physicalArmour = 20;
	int health = 10;

	int maxMana = 0;
	int currentMana = 0;

	UnitZone* attackZone;
	UnitZone* blockZone;

	Sprite deckIcon;
	Texture texDeck;
	Vector2f deckPos = Vector2f(50, 780);

	Sprite gemIcon;
	Texture texGem;
	Vector2f gemPos = Vector2f(50, 620);

	Sprite discardIcon;
	Texture texDiscard;
	Vector2f discardPos = Vector2f(1450, 790);

	Sprite burntIcon;
	Texture texBurnt;
	Vector2f burntPos = Vector2f(1450, 710);

	Sprite physArmIcon;
	Texture texPhysArm;
	Vector2f physArmPos = Vector2f(1410, 350);

	Sprite magArmIcon;
	Texture texMagArm;
	Vector2f magArmPos = Vector2f(1410, 435);

	Sprite healthIcon;
	Texture texHealth;
	Vector2f healthPos = Vector2f(1410, 520);


	Font font;
	Text txtCurrentMana;

	Text txtHealth;
	Text txtPhysArm;
	Text txtMagArm;

	Vector2f txtHealthPos = Vector2f(1500, 540);
	Vector2f txtPhysArmPos = Vector2f(1500, 370);
	Vector2f txtMagArmPos = Vector2f(1500, 455);


	Vector2f attackZonePos = Vector2f(200, 350);
	Vector2f blockZonePos = Vector2f(800, 350);

	vector<UnitZone*> zones;
};

