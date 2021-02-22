#include "Encounter.h"


Encounter::Encounter(RenderWindow* w, DataBase* db, int lvl) {
	database = db;
	level = lvl;

	texIcon.loadFromFile("Textures/GUI/fire.png");
	icon.setTexture(texIcon);

	enemy = new Enemy(w, database);
	GenerateEncounter();
}

void Encounter::GenerateEncounter() {
	string name;
	int r =  rand() %(database->encounterNames[level].size());
	name = database->encounterNames[level][r];
	encData = database->encounters[level][name];

	for (string s : encData->startingPlay) {
		startingPlay.push_back(new Card(*database->CardList[s],database));
	}
	for (string s : encData->decklist) {
		encounterDeck.push_back(new Card(*database->CardList[s], database));
	}
	enemy->SetDetails(encData, startingPlay, encounterDeck);
}