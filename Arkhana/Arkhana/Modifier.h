#pragma once
#include "enums.h"

class Modifier
{
public:
	Modifier();
	~Modifier() {}
	int GetValue() { return value; }
	int GetMultiplier() { return multiplier; }
	STAT_TYPE GetStat() { return sType; }
protected:
	int value = 1;
	int multiplier = 0;
	STAT_TYPE sType = DMG_PHYSICAL;
	MODIFIER_TYPE mType = ZONE_MOD;
};

