#pragma once
#include "GameObject.h"
#include "Card.h"

class Encounter :
    public GameObject
{
public:
    Encounter(DataBase* db, int lvl);
    ~Encounter() {}
    vector<Card*> GetDeck() { return encounterDeck; }
    vector<Card*> GetStartingPlay() { return startingPlay; }
protected:
    void GenerateEncounter();
    DataBase* database;
    vector<Card*> encounterDeck;
    vector<Card*> startingPlay;
    int level = 0;

    EncounterData* encData = new EncounterData();


};

