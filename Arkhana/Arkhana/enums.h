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
	UNIT,
	SPELL
};