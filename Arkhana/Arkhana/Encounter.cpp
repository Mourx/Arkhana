#include "Encounter.h"


Encounter::Encounter(RenderWindow* w, DataBase* db, int lvl) {
	database = db;
	level = lvl;

	texIcon.loadFromFile("Textures/GUI/fire.png");
	icon.setTexture(texIcon);
	cardList = database->CardListEnemy;
	enemy = new Enemy(w, database);
	GenerateEncounter();
}

void Encounter::GenerateEncounter() {
	string name;
	int r =  rand() %(database->encounterNames[level].size());
	name = database->encounterNames[level][r];
	encData = database->encounters[level][name];
	combatDesc = encData->description;
	for (string s : encData->startingPlay) {
		startingPlay.push_back(new Card(*cardList[s],database));
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

void Encounter::SetHover(bool b) {
	if (bHover != true && b == true) {
		database->sound.setBuffer(database->rollover);
		database->sound.play();
	}
	bHover = b;
}