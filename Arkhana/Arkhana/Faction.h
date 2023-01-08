#pragma once
#include "GameObject.h"
#include "Card.h"
class Faction :
    public GameObject
{
public:
    Faction(FactionData* factType);
    ~Faction() {}
    FactionData* GetData() { return type; }
    vector<Card*> GetDecklist() { return deckList; }
    map<string, CardData*> GetCardList() { return cardList; }

protected:
    vector<Card*> deckList;
    FactionData* type;
    map<string, CardData*> cardList;
    
};

