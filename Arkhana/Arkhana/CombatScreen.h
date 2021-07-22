#pragma once
#include "Screen.h"
#include "Card.h"
#include "Player.h"
#include "EndTurnButton.h"
#include "Enemy.h"
#include "Encounter.h"
#include "AIController.h"

class CombatScreen :
    public Screen
{
public:
    CombatScreen(RenderTexture* w, DataBase* data,Player* p,Encounter* enc);
    ~CombatScreen() {}
    void Draw();
    void Update(Time t);
    void MouseMoved(Vector2f mousePos);
    void MouseClicked(Vector2f mousePos);
    void MouseReleased(Vector2f mousePos);
    COMBAT_RESULT GetResult() { return result; }
    Enemy* GetEnemy() { return enemy; }
    void SetInfo(InfoPane* info);
protected:
    Player* player;
    Enemy* enemy;
    Encounter* encounter;
    AIController* AI;
    DataBase* database;
    void AdvanceTurn(COMBAT_TURN turn);
    void CalculateCombat();
    void SetNextEnemyMove();
    void CheckDeaths();

    Card* ePlayedCard = NULL;
    Card* hoverCard = NULL;
    Unit* hoverUnit = NULL;
    Unit* selUnit = NULL;

    float iconOffsetX = 0;
    float iconOffsetY = 0;
    Vector2f iconSnapPos;

    EndTurnButton* endTurn;
    Vector2f endTurnPos = Vector2f(1425, 600);
    
    UnitZone* selectedZone = NULL;
    Card* eNext = NULL;

    COMBAT_TURN currentTurn = PLAYER;

    COMBAT_RESULT result = ONGOING;

    Sprite background;
    Texture texBackground;
};

