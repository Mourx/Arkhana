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
#include <SFML/Audio.hpp>

class Modifier;
using namespace rapidjson;
using namespace std;
using namespace sf;
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
	string effect;
	string unit;
	string zTag;
	string filePath ="";
	ZONE_OWNER zOTag;
	string shaderPath;
	AI_TAG AITag;
	vector<Modifier*> modifiers;
};

struct EffectData {
	string name;
	EFFECT_TYPE effect;
	int value;
	string text;
};

struct ModifierData {
public:
	string name;
	int value;
	int multiplier;
	STAT_TYPE sType;
	string text;
	string mText;
	int EOTChange = 0;
	MODIFIER_TYPE mType;
	Modifier* modifier;
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
	string description;
	ENCOUNTER_TYPE type;
};


class DataBase {
public:
	map<string, UnitData*> UnitList;
	map<string, CardData*> CardListRed;
	map<string, CardData*> CardListEnemy;
	map<string, ModifierData*> modList;
	map<string, EffectData*> effectList;
	vector<map<string, EncounterData*>> encounters;
	vector<vector<string>> encounterNames;
	map<int, string> costIcons;

	bool bDebugMode = true;

	MODIFIER_TYPE GetModEnum(string data) {
		if (data == "ZONE_MOD") return MODIFIER_TYPE::ZONE_MOD;
		else if (data == "UNIT_MOD") return MODIFIER_TYPE::UNIT_MOD;
		else if (data == "INSTANT_MOD") return MODIFIER_TYPE::INSTANT_MOD;
		else if (data == "AURA_MOD") return MODIFIER_TYPE::AURA_MOD;
		else if (data == "BOSS_RESIST_MOD") return MODIFIER_TYPE::BOSS_RESIST_MOD;
		else if (data == "UNIT_EOT_MOD") return MODIFIER_TYPE::UNIT_EOT_MOD;
		else if (data == "AURA_EOT_MOD") return MODIFIER_TYPE::AURA_EOT_MOD;
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
		if (data == "CREATE_UNIT") return CREATE_UNIT;
		else if (data == "APPLY_ZONE_MOD") return APPLY_ZONE_MOD;
		else if (data == "TARGET_STRONGEST") return TARGET_STRONGEST;
		if (data == "CREATE_UNIT_MULTIPLE") return CREATE_UNIT_MULTIPLE;
		else if (data == "EFFECT") return EFFECT;
		
		else return CREATE_UNIT;
	}

	ZONE_TYPE GetZoneEnum(string data) {
		if (data == "AGGRESSIVE") return ZONE_TYPE::Z_ATTACK;
		else if (data == "DEFENSIVE") return ZONE_TYPE::Z_BLOCK;
		else return ZONE_TYPE::Z_ANY;
	}

	ZONE_OWNER GetZoneOwnerEnum(string data) {
		if (data == "Z_PLAYER") return ZONE_OWNER::Z_PLAYER;
		else if (data == "Z_ENEMY") return ZONE_OWNER::Z_ENEMY;
		else return ZONE_OWNER::Z_EITHER;
	}

	AI_TAG GetAITag(string data) {
		if (data == "AI_RANDOM") return AI_RANDOM;
		else if (data == "AI_ATTACK") return AI_ATTACK;
		else if (data == "AI_BLOCK") return AI_BLOCK;
		else if (data == "AI_MOST_UNITS_AI") return AI_MOST_UNITS_AI;
		else if (data == "AI_MOST_UNITS_PLAYER") return AI_MOST_UNITS_PLAYER;
		else if (data == "AI_STRONGEST_UNIT_PLAYER") return AI_STRONGEST_UNIT_PLAYER;
		else return AI_RANDOM;
	}

	EFFECT_TYPE GetEffectEnum(string data) {
		if (data == "AMOUR_MOD") return EFFECT_TYPE::ARMOUR_MOD;
		else return EFFECT_TYPE::ARMOUR_MOD;
	}

	ENCOUNTER_TYPE GetEncType(string data) {
		if (data == "E_COMBAT") return E_COMBAT;
		else if (data == "E_GIFT") return E_GIFT;
		else if (data == "E_EVENT") return E_EVENT;
		else if (data == "E_ELITE") return E_ELITE;
		else if (data == "E_BOSS") return E_BOSS;
	}

	void BuildUnitLists();
	void BuildModifierLists();
	void BuildCardListsRed();
	void BuildCardListsEnemy();
	void BuildEncounterLists();
	void BuildEffectLists();
	void Init();



	Sound sound;
	SoundBuffer rollover;
};

