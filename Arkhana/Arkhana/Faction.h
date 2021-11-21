#pragma once
#include "GameObject.h"
#include "DataBase.h"
class Faction :
    public GameObject
{
public:
    Faction(DataBase* db, FACTION_TYPE factType);
    ~Faction() {}
    FACTION_TYPE GetType() { return type; }
protected:
    DataBase* database;
    vector<CardData*> startingDeck;
    vector<CardData*> decklist;
    FACTION_TYPE type;
};

