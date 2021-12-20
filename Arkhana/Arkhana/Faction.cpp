#include "Faction.h"


Faction::Faction(DataBase* db, FactionData* factType) {
	database = db;
	type = factType;
	texIcon.loadFromFile(type->filepath);
	icon.setTexture(texIcon);
	icon.setScale(3, 3);
	cardList = database->CardListRedUnlocked;
	for (string name : type->decklist) {
		deckList.push_back(new Card(*cardList[name], database));
	}
}