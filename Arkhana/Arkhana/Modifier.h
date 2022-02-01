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
	void SetPosition(Vector2f pos) { 
		icon.setPosition(pos);
		FloatRect tR = textDuration.getLocalBounds();
		textDuration.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
		textDuration.setPosition(pos + Vector2f(15, 15));
	}
	void Draw(RenderTexture* w) {
		w->draw(icon);
		if (duration <= 10) {
			w->draw(textDuration);
		}
	}
	Modifier* GetModifier() { return modifier; }
	void ModifyDuration(int value) {
		duration += value;
		textDuration.setString(to_string(duration));
	}
	int GetDuration() {
		return duration;
	}
	void SetValue(int v) {
		value = v;
	}
	void SetName(string s) {
		name = s;
	}
	void SetMText(string s) {
		mText = s;
	}
protected:
	
	void GetData();
	
	ModifierData data;
	string name;
	string text;
	string mText;
	int value = 1;
	int duration = 999;
	int multiplier = 0;
	int EOTChange = 0;
	STAT_TYPE sType = STAT_TYPE::DMG_PHYSICAL;
	MODIFIER_TYPE mType = MODIFIER_TYPE::ZONE_MOD;
	Modifier* modifier;
	Sprite icon;
	Texture texIcon;
	Text textDuration;
	Font font;

};

