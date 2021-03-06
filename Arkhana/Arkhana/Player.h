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
	Player(RenderTexture* w,DataBase* data);
	~Player() {}
	void Setup();
	void DrawBackground();
	void DrawForeground();
	void DrawPlayerBar();
	void Update(Time t);

	vector<Card*> GetHand() { return hand; }
	int GetHealth() { return health; }
	Card* selectedCard = NULL;
	void UseCard(Card* c);
	int GetCurrentMana() { return currentMana; }
	vector<UnitZone*> GetZones() { return zones; }
	virtual void NewTurnUpkeep();
	virtual void EndTurnUpkeep();
	void DamagePhys(int damage);
	void ResetMana() { currentMana = maxMana; }
	void AnimateAttack();
	bool GetAttacking() { return bAttacking; }
	bool HasAttacked() { return bHasAttacked; }
	vector<Card*> GetDeckList() { return decklist; }
	void ModifyArmour(int val) { armour += val; }
	map<string, CardData*> GetCardLists() { return cardList; }
	void AddCard(Card* c) { decklist.push_back(c); }
	void AddGold(int m) { currentGold += m; }
	void SetFaction();
	int GetGold() { return currentGold; }
	void SetBarWindow(RenderTexture* w) {
		playerBarWindow = w;
	}
protected:
	void DiscardHand();
	void DrawCards(int amount);
	void Discard(Card* card);
	void Discard(int index);
	void DrawInitialHand();
	void SetCardPositions();
	virtual void InitSprites();
	virtual void UpdateStrings();
	DataBase* database;
	vector<Card*> decklist;
	vector<Card*> deck;
	vector<Card*> hand;
	vector<Card*> discard;
	vector<Card*> burnt;
	random_device random;

	Vector2f cardsStart = Vector2f(200, 700);
	Vector2f cardsEnd = Vector2f(1250, 700);
	Vector2f offScreenPos = Vector2f(-200, -200);

	RenderTexture* window;
	RenderTexture* playerBarWindow;
	int CARDS_PER_TURN = 5;
	int CARDS_START = 6;

	int maxArmour = 20;
	int armour = maxArmour;
	int health = 10;
	int maxHealth = 50;

	int maxMana = 3;
	int currentMana = 3;

	int currentGold = 30;

	Text txtCurMana;
	Vector2f txtCurManaPos = Vector2f(100,700);

	UnitZone* attackZone;
	UnitZone* blockZone;

	Sprite deckIcon;
	Texture texDeck;
	Vector2f deckPos = Vector2f(50, 780);

	Text txtDeckSize;
	Vector2f txtDeckSizePos = Vector2f(100, 830);

	Text txtDiscardSize;
	Vector2f txtDiscardSizePos = Vector2f(1490, 840);

	Text txtBurntSize;
	Vector2f txtBurntSizePos = Vector2f(1490, 750);

	Sprite gemIcon;
	Texture texGem;
	Vector2f gemPos = Vector2f(50, 620);

	Sprite discardIcon;
	Texture texDiscard;
	Vector2f discardPos = Vector2f(1450, 790);

	Sprite burntIcon;
	Texture texBurnt;
	Vector2f burntPos = Vector2f(1450, 710);

	Sprite playerBarIcon;
	Texture texPlayerBar;
	Vector2f playerBarPos = Vector2f(0, 0);

	Sprite playerGoldIcon;
	Texture texPlayerGold;
	Vector2f playerGoldPos = Vector2f(400, 50);
	Text txtPlayerGold;
	Vector2f txtPlayerGoldPos = playerGoldPos + Vector2f(85, 20);

	Sprite deckSizeIcon;
	Vector2f deckSizePos = Vector2f(600, 40);
	Text txtDeckTotalSize;
	Vector2f txtDeckTotalSizePos = deckSizePos + Vector2f(110, 50);

	Sprite physArmIcon;
	Texture texPhysArm;
	Vector2f physArmPos = Vector2f(1410, 350);

	Sprite healthIcon;
	Texture texHealth;
	Vector2f healthPos = Vector2f(200, 50);

	float attackDuration = 1;
	float attackTimer = 0;
	float attackDirection = -1;
	bool bAttacking = false;
	bool bHasAttacked = false;

	float playDuration = 2;
	float playTimer = 0;
	bool bPlayingCard = false;


	Font font;
	Text txtCurrentMana;

	Text txtHealth;
	Text txtPhysArm;

	Vector2f txtHealthPos = healthPos + Vector2f(85, 20);
	Vector2f txtPhysArmPos = physArmPos + Vector2f(70, 150);

	Vector2f attackZonePos = Vector2f(200, 350);
	Vector2f blockZonePos = Vector2f(800, 350);

	vector<UnitZone*> zones;
	map<string,CardData*> cardList;
};

