#pragma once
#include "GameObject.h"
#include "Card.h"
#include "Enemy.h"

class Encounter :
    public GameObject
{
public:
    Encounter(RenderTexture* w, DataBase* db, int lvl);
    ~Encounter() {}
    vector<vector<Card*>> GetDecks() { return encounterDecks; }
    vector<Card*> GetStartingPlay() { return startingPlay; }
    void SetComplete(bool b) { bComplete = b; }
    bool GetComplete() { return bComplete; }
    Enemy* GetEnemy() { return enemy; }
    String GetEncDescription() { return desc; }
    String GetCombatDescription() { return combatDesc; }
    String GetTypeDesc() { return typeDesc; }
    String GetTypeTitle() { return typeTitle; }
    int GetLevel() { return level; }
    void SetHover(bool b);
protected:
    void GenerateEncounter();
    void SetupCombat();
    DataBase* database;
    vector<vector<Card*>> encounterDecks;
    vector<Card*> startingPlay;
    int level = 0;
    bool bComplete = false;
    EncounterData* encData = new EncounterData();
    RenderTexture* window;
    String desc = "An encounter with \nenemies.";
    String combatDesc = "enemies lol";
    String typeDesc = "A fight against\nregular enemies";
    String typeTitle = "Normal Combat";
    Enemy* enemy;
    map<string, CardData*> cardList;
    ENCOUNTER_TYPE eType;
};

