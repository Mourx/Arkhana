#pragma once
#include "enums.h"
#include "DataBase.h"
#include <SFML/Graphics.hpp>

class Modifier
{
public:
	Modifier(ModifierData d);
	Modifier(Modifier*);
	~Modifier() {}
	int GetValue() { return value; }
	int GetMultiplier() { return multiplier; }
	STAT_TYPE GetStat() { return sType; }
	MODIFIER_TYPE GetModType() { return mType; }
	string GetText() { return text; }
	string GetShortText() { return mText; }
	void ApplyEOT() { value += EOTChange; }
	int GetEOT() { return EOTChange; }
	string GetName() { return name; }
	void SetPosition(Vector2f pos) { icon.setPosition(pos); }
	void Draw(RenderTexture* w) {
		w->draw(icon);
	}
	vector<Modifier*> GetModifiers() { return modifiers; }
protected:
	
	void GetData();
	
	ModifierData data;
	string name;
	string text;
	string mText;
	int value = 1;
	int multiplier = 0;
	int EOTChange = 0;
	STAT_TYPE sType = STAT_TYPE::DMG_PHYSICAL;
	MODIFIER_TYPE mType = MODIFIER_TYPE::ZONE_MOD;
	vector<Modifier*> modifiers;
	Sprite icon;
	Texture texIcon;


};

