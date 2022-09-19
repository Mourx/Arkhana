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
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Modifier;
class Player;
class Faction;
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
	vector<string> effect;
	string unit;
	string zTag;
	string filePath ="";
	ZONE_OWNER zOTag;
	string shaderPath;
	AI_TAG AITag;
	int level = 0;
	vector<Modifier*> modifiers;
	vector<string> cards;
	RARITY rarity;
};

struct EffectData {
	string name;
	EFFECT_TYPE effect;
	int value;
	string text;
	bool tUnit = false;
	string unit;
};

struct ModifierData {
public:
	string name;
	int value;
	float multiplier;
	STAT_TYPE sType;
	string text;
	string mText;
	int EOTChange = 0;
	MODIFIER_TYPE mType;
	vector<Modifier*> modifiers;
	int duration;
	string filePath;
	bool bDynamic = false;
	vector<string> effect;
	vector<string> except;
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

struct FactionData {
	vector<string> decklist;
	string filepath;
	string name;
};

class DataBase {
public:
	map<string, UnitData*> UnitList;
	map<string, CardData*> CardListRedUnlocked;
	map<string, CardData*> CardListRedAll;
	vector<string> cardsCommon;
	vector<string> cardsUncommon;
	vector<string> cardsRare;
	map<string, CardData*> CardListEnemy;
	map<string, CardData*> CardListAll;
	map<string, ModifierData*> modList;
	map<string, EffectData*> effectList;
	map<string, Texture*> texList;
	vector<FactionData*> factionList;
	vector<map<string, EncounterData*>> encounters;
	vector<vector<string>> encounterNames;
	map<int, string> costIcons;

	int playerLevel = 1;
	bool bDebugMode = false;

	Font font;
	Font coolFont;

	string GetRandomCard(RARITY rarity) {
		switch (rarity) {
		case COMMON:
			return cardsCommon[rand() % cardsCommon.size()];
		case UNCOMMON:
			return cardsUncommon[rand() % cardsUncommon.size()];
		case RARE:
			return cardsRare[rand() % cardsRare.size()];
		}
	}


	MODIFIER_TYPE GetModEnum(string data) {
		if (data == "ZONE_MOD") return MODIFIER_TYPE::ZONE_MOD;
		else if (data == "UNIT_MOD") return MODIFIER_TYPE::UNIT_MOD;
		else if (data == "INSTANT_MOD") return MODIFIER_TYPE::INSTANT_MOD;
		else if (data == "AURA_MOD") return MODIFIER_TYPE::AURA_MOD;
		else if (data == "BOSS_RESIST_MOD") return MODIFIER_TYPE::BOSS_RESIST_MOD;
		else if (data == "UNIT_EOT_MOD") return MODIFIER_TYPE::UNIT_EOT_MOD;
		else if (data == "AURA_EOT_MOD") return MODIFIER_TYPE::AURA_EOT_MOD;
		else if (data == "PLAYER_EOT_MOD") return MODIFIER_TYPE::PLAYER_EOT_MOD;
		else if (data == "PLAYER_MOD") return MODIFIER_TYPE::PLAYER_MOD;
		else if (data == "SWAP_ZONE") return MODIFIER_TYPE::SWAP_ZONE;
		else if (data == "ZONE_MOD_ATTACK") return MODIFIER_TYPE::ZONE_MOD_ATTACK;
		else if (data == "MUSIC_AURA") return MODIFIER_TYPE::MUSIC_AURA;
		else if (data == "OPPOSITE_ZONE") return MODIFIER_TYPE::OPPOSITE_ZONE;
		else if (data == "SOT_EFFECT") return MODIFIER_TYPE::SOT_EFFECT;
		else if (data == "UNIT_ENTER_APPLY_MOD") return MODIFIER_TYPE::UNIT_ENTER_APPLY_MOD;
		else if (data == "UNIT_SOT_MOD") return MODIFIER_TYPE::UNIT_SOT_MOD;
		else if (data == "EOT_EFFECT") return MODIFIER_TYPE::EOT_EFFECT;
		else if (data == "UNIT_LEAVE_MOD") return MODIFIER_TYPE::UNIT_LEAVE_MOD;
		else if (data == "SET_ATTACK") return MODIFIER_TYPE::SET_ATTACK;
		else if (data == "SET_STAMINA") return MODIFIER_TYPE::SET_STAMINA;
		else if (data == "EXHAUST_EFFECT") return MODIFIER_TYPE::EXHAUST_EFFECT;
		else return MODIFIER_TYPE::ZONE_MOD;
	}

	STAT_TYPE GetStatEnum(string data) {
		if (data == "DMG_PHYSICAL") return STAT_TYPE::DMG_PHYSICAL;
		else if (data == "STAMINA") return STAT_TYPE::STAMINA;
		else if (data == "ARMOUR_PHYSICAL") return STAT_TYPE::ARMOUR_PHYSICAL;
		else if (data == "HEALTH") return STAT_TYPE::HEALTH;
		else if (data == "EFFECT_COST") return STAT_TYPE::EFFECT_COST;
		else if (data == "UNIT_COST") return STAT_TYPE::UNIT_COST;
		else if (data == "ZONE_POWER") return STAT_TYPE::ZONE_POWER;
		else if (data == "MAX_MANA") return STAT_TYPE::MAX_MANA;
		else return STAT_TYPE::DMG_PHYSICAL;
	}

	CARD_TYPE GetCardEnum(string data) {
		if (data == "UNIT") return UNIT;
		else if (data == "EFFECT") return EFFECT;
		else return UNIT;
	}

	RARITY GetRarityEnum(string data) {
		if (data == "COMMON") return RARITY::COMMON;
		if (data == "UNCOMMON") return RARITY::UNCOMMON;
		if (data == "RARE") return RARITY::RARE;
		else return COMMON;
	}

	ZONE_TYPE GetZoneEnum(string data) {
		if (data == "AGGRESSIVE") return ZONE_TYPE::Z_ATTACK;
		else if (data == "DEFENSIVE") return ZONE_TYPE::Z_BLOCK;
		else if (data == "Z_MIN_3_UNIT") return ZONE_TYPE::Z_MIN_3_UNIT;
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
		if (data == "ZONE_MOD") return EFFECT_TYPE::ZONE_MOD;
		if (data == "TARGET_STRONGEST") return EFFECT_TYPE::TARGET_STRONGEST;
		if (data == "TARGET_UNIT") return EFFECT_TYPE::TARGET_UNIT;
		if (data == "FROG_FRIENDS") return EFFECT_TYPE::FROG_FRIENDS;
		if (data == "POWER_RIBBIT") return EFFECT_TYPE::POWER_RIBBIT;
		if (data == "CREATE_UNIT") return EFFECT_TYPE::CREATE_UNIT;
		if (data == "FROG_DOG") return EFFECT_TYPE::FROG_DOG;
		if (data == "MODIFY_BOTH_BLOCK") return EFFECT_TYPE::MODFIY_BOTH_BLOCK;
		if (data == "DRIZZLE") return EFFECT_TYPE::DRIZZLE;
		if (data == "POTION_BUFF") return EFFECT_TYPE::POTION_BUFF;
		if (data == "MODIFY_ALL_ATTACK") return EFFECT_TYPE::MODIFY_ALL_ATTACK;
		if (data == "APPLY_MOD") return EFFECT_TYPE::APPLY_MOD;
		if (data == "GAIN_CARD") return EFFECT_TYPE::GAIN_CARD;
		if (data == "GIVE_CARD") return EFFECT_TYPE::GIVE_CARD;
		if (data == "DAMAGE") return EFFECT_TYPE::DAMAGE;
		if (data == "CHORUS_CROAK") return EFFECT_TYPE::CHORUS_CROAK;
		if (data == "FEAR_FROG") return EFFECT_TYPE::FEAR_FROG;
		if (data == "MODIFY_ZONE_STAMINA") return EFFECT_TYPE::MODIFY_ZONE_STAMINA;
		if (data == "MODIFY_ZONE_ATTACK") return EFFECT_TYPE::MODIFY_ZONE_ATTACK;
		if (data == "FROGAPULT") return EFFECT_TYPE::FROGAPULT;
		if (data == "JUNGLE_FROG") return EFFECT_TYPE::JUNGLE_FROG;
		if (data == "FIRE_FROG") return EFFECT_TYPE::FIRE_FROG;
		if (data == "BUILDER_FROG") return EFFECT_TYPE::BUILDER_FROG;
		if (data == "CREATE_UNIT_EOT") return EFFECT_TYPE::CREATE_UNIT_EOT;
		if (data == "TWISTER") return EFFECT_TYPE::TWISTER;
		if (data == "GOD_FROG") return EFFECT_TYPE::GOD_FROG;
		if (data == "MODIFY_GOLD") return EFFECT_TYPE::MODIFY_GOLD;
		if (data == "DRAW_CARD") return EFFECT_TYPE::DRAW_CARD;
		if (data == "DISCARD_CARD") return EFFECT_TYPE::DISCARD_CARD;
		if (data == "MODIFY_HEALTH") return EFFECT_TYPE::MODIFY_HEALTH;
		if (data == "MODIFY_ALL") return EFFECT_TYPE::MODIFY_ALL;
		if (data == "CREATE_UNIT_EXHAUST") return EFFECT_TYPE::CREATE_UNIT_EXHAUST;
		if (data == "MOVE_ALLIES") return EFFECT_TYPE::MOVE_ALLIES;
		if (data == "ADD_ZONE_MOD") return EFFECT_TYPE::ADD_ZONE_MOD;
		if (data == "USE_ONCE") return EFFECT_TYPE::USE_ONCE;
		if (data == "DAMAGE_ON_DRAW") return EFFECT_TYPE::DAMAGE_ON_DRAW;
		if (data == "GIVE_CARD_DECK") return EFFECT_TYPE::GIVE_CARD_DECK;
		if (data == "CREATE_UNIT_OPPOSITE") return EFFECT_TYPE::CREATE_UNIT_OPPOSITE;	
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
	void BuildFactionLists();
	void BuildAllCardList();
	void BuildSharedTextures();
	void Init();
	Player* player;
	Player* enemy;

	Sound sound;
	SoundBuffer rollover;

	vector<GameObject*> Tutorials;
	vector<Texture*> TexTutorials;
	GameObject* tutorialButton;
	GameObject* nextButton;
	GameObject* backButton;
	GameObject* closeButton;
	int tutIndex = 0;

	void GetTut(int dir) {
		if (tutIndex + dir < 0 || tutIndex + dir >= Tutorials.size()) {
		}
		else {
			tutIndex += dir;
		}
	}

	vector<string> retreatPhrases;
	vector<Faction*> factions;
};



