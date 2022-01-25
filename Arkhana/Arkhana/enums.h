#pragma once



enum COMBAT_STATE {
	MAIN = 0,
	COMBAT = 1,
	CLEANUP = 2
};

enum COMBAT_RESULT {
	LOSS = 0,
	WIN = 1,
	ONGOING = 2
};

enum NEXT_SCREEN {
	MAIN_MENU,
	COMBAT_SCREEN,
	GAME_OVER,
	REWARD_SCREEN,
	PATH_SCREEN,
	FORGE_SCREEN,
	NONE
};

enum COMBAT_TURN {
	PLAYER = 0,
	ENEMY = 1
};

enum CARD_TYPE {
	UNIT = 0,
	EFFECT = 1,
};

enum ZONE_OWNER {
	Z_PLAYER,
	Z_ENEMY,
	Z_EITHER
};

enum class ZONE_TYPE {
	Z_ANY = 3,
	Z_ATTACK = 0,
	Z_BLOCK =1
};

enum AI_TAG {
	AI_RANDOM,
	AI_ATTACK,
	AI_BLOCK,
	AI_MOST_UNITS_AI,
	AI_MOST_UNITS_PLAYER,
	AI_STRONGEST_UNIT_PLAYER
};

enum class MODIFIER_TYPE {
	ZONE_MOD = 0,
	UNIT_MOD = 1,
	INSTANT_MOD = 2,
	AURA_MOD = 3,
	BOSS_RESIST_MOD = 4,
	UNIT_EOT_MOD,
	AURA_EOT_MOD,
	PLAYER_EOT_MOD,
	PLAYER_MOD,
	SWAP_ZONE,
	ZONE_MOD_ATTACK,
	MUSIC_AURA,
	OPPOSITE_ZONE,
	SOT_EFFECT,
	UNIT_ENTER_APPLY_MOD,
	UNIT_SOT_MOD
};

enum class EFFECT_TYPE {
	ARMOUR_MOD,
	ZONE_MOD,
	TARGET_STRONGEST,
	TARGET_UNIT,
	CREATE_UNIT,
	FROG_FRIENDS,
	POWER_RIBBIT,
	FROG_DOG,
	MODFIY_BOTH_BLOCK,
	DRIZZLE
};

enum class STAT_TYPE {
	DMG_PHYSICAL,
	STAMINA,
	ARMOUR_PHYSICAL,
	HEALTH,
	EFFECT_COST,
	UNIT_COST,
	ZONE_POWER
};

enum ENCOUNTER_TYPE {
	E_COMBAT,
	E_GIFT,
	E_EVENT,
	E_ELITE,
	E_BOSS
};

enum TRANSITION_TYPE {
	LEFT_ENTER,
	LEFT_EXIT,
	DOWN_ENTER,
	DOWN_EXIT
};

enum FACTION_TYPE {
	FROG_BASIC
};