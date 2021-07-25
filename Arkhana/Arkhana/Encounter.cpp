#include "Encounter.h"


Encounter::Encounter(RenderTexture* w, DataBase* db, int lvl) {
	database = db;
	level = lvl;
	window = w;
	GenerateEncounter();
		
	icon.setTexture(texIcon);
	
	
}

void Encounter::GenerateEncounter() {
	string name;
	int r =  rand() %(database->encounterNames[level].size());
	name = database->encounterNames[level][r];
	encData = database->encounters[level][name];
	eType = encData->type;

	switch (eType) {
	case E_COMBAT:
		SetupCombat();
		texIcon.loadFromFile("Textures/GUI/swordsCombat.png");
		typeDesc = "A fight against\nregular enemies";
		typeTitle = "Normal Combat";
		break;
	case E_BOSS:
		SetupCombat();
		texIcon.loadFromFile("Textures/GUI/bossSkulls.png");
		typeDesc = "A hard fight against\ndeadly enemies";
		typeTitle = "Boss Combat";
		break;
	case E_GIFT:
		texIcon.loadFromFile("Textures/GUI/chestGift.png");
		typeDesc = "What treasure awaits?";
		typeTitle = "A Gift of Treasure";
		break;
	}
	
}

void Encounter::SetHover(bool b) {
	if (bHover != true && b == true) {
		database->sound.setBuffer(database->rollover);
		database->sound.play();
	}
	bHover = b;
}

void Encounter::SetupCombat() {
	cardList = database->CardListEnemy;
	enemy = new Enemy(window, database);

	combatDesc = encData->description;
	for (string s : encData->startingPlay) {
		startingPlay.push_back(new Card(*cardList[s], database));
	}
	for (vector<string> v : encData->decklists) {
		vector<Card*> deck;
		for (string s : v) {
			deck.push_back(new Card(*cardList[s], database));
		}
		encounterDecks.push_back(deck);
	}
	enemy->SetDetails(encData, startingPlay, encounterDecks);
}