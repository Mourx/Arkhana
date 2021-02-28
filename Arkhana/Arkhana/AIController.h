#pragma once
#include "Enemy.h"

class AIController
{
public:
	AIController(Enemy* enemy, Player* p);
	~AIController() {};
	UnitZone* GetTarget(Card* c);
protected:
	Enemy* AIEnemy;
	Player* player;
	UnitZone* AIAttackZone;
	UnitZone* AIBlockZone;
	UnitZone* PlayerAttackZone;
	UnitZone* PlayerBlockZone;
};

