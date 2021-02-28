#pragma once
#include <string>
#include <vector>
#include <map>
#include "enums.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>
#include <cstdio>
#include <iostream>
class Modifier;
using namespace rapidjson;
using namespace std;

struct UnitData {
public:
	string name;
	int physPower;
	int stamina;
	string filePath;
};

struct CardData {
public:
	string name;
	int cost;
	CARD_TYPE cType;
	string unit;
	string zTag;
	string shaderPath;
	AI_TAG AITag;
	vector<Modifier*> modifiers;
};

struct ModifierData {
public:
	string name;
	int value;
	int multiplier;
	STAT_TYPE sType;
	string text;
	MODIFIER_TYPE mType;
};

struct EncounterData {
public:
	string name;
	int health;
	int armour;
	vector<vector<string>> decklists;
	vector<string> startingPlay;
	int level;
	int actionCount;
};


class DataBase {
public:
	map<string, UnitData*> UnitList;
	map<string, CardData*> CardList;
	map<string, ModifierData*> modList;
	vector<map<string, EncounterData*>> encounters;
	vector<vector<string>> encounterNames;
	map<int, string> costIcons;

	MODIFIER_TYPE GetModEnum(string data) {
		if (data == "ZONE_MOD") return MODIFIER_TYPE::ZONE_MOD;
		else if (data == "UNIT_MOD") return MODIFIER_TYPE::UNIT_MOD;
		else if (data == "INSTANT_MOD") return MODIFIER_TYPE::INSTANT_MOD;
		else if (data == "AURA_MOD") return MODIFIER_TYPE::AURA_MOD;
		else return MODIFIER_TYPE::ZONE_MOD;
	}

	STAT_TYPE GetStatEnum(string data) {
		if (data == "DMG_PHYSICAL") return STAT_TYPE::DMG_PHYSICAL;
		else if (data == "STAMINA") return STAT_TYPE::STAMINA;
		else if (data == "ARMOUR_PHYSICAL") return STAT_TYPE::ARMOUR_PHYSICAL;
		else if (data == "HEALTH") return STAT_TYPE::HEALTH;
		else return STAT_TYPE::DMG_PHYSICAL;
	}

	CARD_TYPE GetCardEnum(string data) {
		if (data == "UNIT") return UNIT;
		else if (data == "SPELL") return SPELL;
		
		else return UNIT;
	}

	ZONE_TYPE GetZoneEnum(string data) {
		if (data == "AGGRESSIVE") return ZONE_TYPE::Z_ATTACK;
		else if (data == "DEFENSIVE") return ZONE_TYPE::Z_BLOCK;
		else return ZONE_TYPE::Z_ANY;
	}

	AI_TAG GetAITag(string data) {
		if (data == "AI_RANDOM") return AI_RANDOM;
		else if (data == "AI_ATTACK") return AI_ATTACK;
		else if (data == "AI_BLOCK") return AI_BLOCK;
		else if (data == "AI_MOST_UNITS_AI") return AI_MOST_UNITS_AI;
		else if (data == "AI_MOST_UNITS_PLAYER") return AI_MOST_UNITS_PLAYER;
		else return AI_RANDOM;
	}

	void BuildUnitLists();
	void BuildModifierLists();
	void BuildCardLists();
	void BuildEncounterLists();
	void Init();
};

