#pragma once
#include "GameObject.h"
#include "DataBase.h"
#include "Card.h"
class Faction :
    public GameObject
{
public:
    Faction(DataBase* db, FactionData* factType);
    ~Faction() {}
    FactionData* GetData() { return type; }
    vector<Card*> GetDecklist() { return deckList; }
    map<string, CardData*> GetCardList() { return cardList; }

protected:
    DataBase* database;
    vector<Card*> deckList;
    FactionData* type;
    map<string, CardData*> cardList;
    
};

