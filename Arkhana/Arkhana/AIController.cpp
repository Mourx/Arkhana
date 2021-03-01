#include "AIController.h"

AIController::AIController(Enemy* enemy, Player* p){
	AIEnemy = enemy;
	player = p;

	AIAttackZone = AIEnemy->GetZones()[(int)ZONE_TYPE::Z_ATTACK];
	AIBlockZone = AIEnemy->GetZones()[(int)ZONE_TYPE::Z_BLOCK];

	PlayerAttackZone = player->GetZones()[(int)ZONE_TYPE::Z_ATTACK];
	PlayerBlockZone = player->GetZones()[(int)ZONE_TYPE::Z_BLOCK];
}

UnitZone* AIController::GetTarget(Card* c) {
	int r = 0;
	switch (c->GetAITag()) {
	case AI_ATTACK:
		return AIAttackZone;
	case AI_BLOCK:
		return AIBlockZone;
	case AI_RANDOM:
		r = rand() % 3;
		if (r <= 1) return AIAttackZone;
		else return AIBlockZone;
	case AI_MOST_UNITS_AI:
		if (AIAttackZone->GetUnits().size() >= AIBlockZone->GetUnits().size()) {
			return AIAttackZone;
		}
		else return AIBlockZone;
	case AI_MOST_UNITS_PLAYER:
		if (PlayerAttackZone->GetUnits().size() >= PlayerBlockZone->GetUnits().size()) {
			return PlayerAttackZone;
		}
		else return PlayerBlockZone;
	case AI_STRONGEST_UNIT_PLAYER:
		if (PlayerBlockZone->GetStrongestPower() > PlayerAttackZone->GetStrongestPower()) {
			return PlayerBlockZone;
		}
		else return PlayerAttackZone;
	}
}