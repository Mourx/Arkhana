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
    CombatScreen(RenderTexture* w,Player* p,Encounter* enc);
    ~CombatScreen() {}
    void Draw();
    void Update(Time t);
    void MouseMoved(Vector2f mousePos);
    void MouseClicked(Vector2f mousePos);
    void MouseReleased(Vector2f mousePos);
    COMBAT_RESULT GetResult() { return result; }
    Enemy* GetEnemy() { return enemy; }
    void SetInfo(InfoPane* info);
    void UpdateDamagePredictions();
protected:
    Player* player;
    Enemy* enemy;
    Encounter* encounter;
    AIController* AI;
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
    Vector2f endTurnPos = Vector2f(1450, 600);
    
    UnitZone* selectedZone = NULL;
    Card* eNext = NULL;

    COMBAT_TURN currentTurn = PLAYER;

    COMBAT_RESULT result = ONGOING;

    Sprite background;
    Texture texBackground;

    Sprite playerDamageSign;
    Sprite enemyDamageSign;
    Texture texPDS;

    Vector2f pdsPos = Vector2f(5,400);
    Vector2f edsPos = Vector2f(1270, 250);

    Text textPAttack;
    Text textPBlock;
    Text textEAttack;
    Text textEBlock;

    Text textEstDamagePlayer;
    Text textEstDamageEnemy;

    Vector2f estDamagePlayerPos = Vector2f(1330, 605);
    Vector2f estDamageEnemyPos = Vector2f(1330, 185);

    Shader shaderTranslucent;

    Vector2f textPAttackPos = Vector2f(900, 525);
    Vector2f textPBlockPos = Vector2f(300, 525);
    Vector2f textEAttackPos = Vector2f(300, 175);
    Vector2f textEBlockPos = Vector2f(900, 175);

};

