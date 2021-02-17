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
	int magPower;
	string filePath;
};

struct CardData {
public:
	string name;
	int cost;
	CARD_TYPE cType;
	int pPow;
	int mPow;
	string unit;
	string filePath;
	string shaderPath;
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
	vector<string> decklist;
	vector<string> startingPlay;
	int level;
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
		else if (data == "DMG_MAGIC") return STAT_TYPE::DMG_MAGIC;
		else if (data == "ARMOUR_PHYSICAL") return STAT_TYPE::ARMOUR_PHYSICAL;
		else if (data == "ARMOUR_MAGIC") return STAT_TYPE::ARMOUR_MAGIC;
		else if (data == "HEALTH") return STAT_TYPE::HEALTH;
		else return STAT_TYPE::DMG_PHYSICAL;
	}

	CARD_TYPE GetCardEnum(string data) {
		if (data == "UNIT") return UNIT;
		else if (data == "SPELL") return SPELL;
		
		else return UNIT;
	}

	void BuildUnitLists();
	void BuildModifierLists();
	void BuildCardLists();
	void BuildEncounterLists();
	void Init();
};

