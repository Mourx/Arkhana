#pragma once
#include "GameObject.h"
#include "Card.h"
#include "Enemy.h"

class Encounter :
    public GameObject
{
public:
    Encounter(RenderWindow* w, DataBase* db, int lvl);
    ~Encounter() {}
    vector<Card*> GetDeck() { return encounterDeck; }
    vector<Card*> GetStartingPlay() { return startingPlay; }
    void SetComplete(bool b) { bComplete = b; }
    bool GetComplete() { return bComplete; }
    Enemy* GetEnemy() { return enemy; }
protected:
    void GenerateEncounter();
    DataBase* database;
    vector<Card*> encounterDeck;
    vector<Card*> startingPlay;
    int level = 0;
    bool bComplete = false;
    EncounterData* encData = new EncounterData();

    Enemy* enemy;
};
