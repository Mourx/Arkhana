#pragma once
#include "enums.h"
#include "DataBase.h"


class Modifier
{
public:
	Modifier(ModifierData data);
	~Modifier() {}
	int GetValue() { return value; }
	int GetMultiplier() { return multiplier; }
	STAT_TYPE GetStat() { return sType; }
	MODIFIER_TYPE GetModType() { return mType; }
	string GetText() { return text; }
protected:
	string name;
	string text;
	int value = 1;
	int multiplier = 0;
	STAT_TYPE sType = STAT_TYPE::DMG_PHYSICAL;
	MODIFIER_TYPE mType = MODIFIER_TYPE::ZONE_MOD;
};

