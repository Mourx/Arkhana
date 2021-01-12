#include "Modifier.h"


Modifier::Modifier(int ID) {
	if (ID == 0) {
		mType = MODIFIER_TYPE::ZONE_MOD;
		sType = STAT_TYPE::DMG_MAGIC;
	}
	else {
		mType = MODIFIER_TYPE::AURA_MOD;
		sType = STAT_TYPE::DMG_MAGIC;
	}
}