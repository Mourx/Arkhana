#pragma once



enum COMBAT_STATE {
	MAIN = 0,
	COMBAT = 1,
	CLEANUP = 2
};

enum COMBAT_RESULT {
	LOSS = 0,
	WIN = 1
};

enum COMBAT_TURN {
	PLAYER = 0,
	ENEMY = 1
};

enum CARD_TYPE {
	UNIT = 0,
	SPELL = 1
};

enum ZONE_OWNER {
	Z_PLAYER = 0,
	Z_ENEMY = 1
};

enum class ZONE_TYPE {
	Z_ATTACK = 0,
	Z_BLOCK =1
};

enum class MODIFIER_TYPE {
	ZONE_MOD = 0,
	UNIT_MOD = 1,
	INSTANT_MOD = 2,
	AURA_MOD
};

enum class STAT_TYPE {
	DMG_PHYSICAL,
	DMG_MAGIC,
	ARMOUR_PHYSICAL,
	ARMOUR_MAGIC,
	HEALTH
};

enum UNIT_NAME {
	GOBLIN,
	SHRINE
};