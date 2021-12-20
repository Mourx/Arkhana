#include "DataBase.h"
#include "Modifier.h"
#include "Faction.h"

void DataBase::BuildModifierLists() {
	FILE* fp;
	fopen_s(&fp, "Data/Modifiers.json", "rb");

	char readBuffer[16384];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
	d.ParseStream(is);

	for (Value::ConstMemberIterator ListItr = d["Modifiers"].MemberBegin(); ListItr != d["Modifiers"].MemberEnd(); ListItr++) {
		ModifierData* mod = new ModifierData();
		mod->name = ListItr->value["name"].GetString();
		mod->value = ListItr->value["value"].GetInt();
		mod->multiplier = ListItr->value["multiplier"].GetInt();
		mod->sType = GetStatEnum(ListItr->value["sType"].GetString());
		mod->mType = GetModEnum(ListItr->value["mType"].GetString());
		if (ListItr->value.HasMember("EOTChange")) mod->EOTChange = ListItr->value["EOTChange"].GetInt();
		if (ListItr->value.HasMember("duration")) {
			int dur = ListItr->value["duration"].GetInt();
			mod->duration = dur;
		}
		else {
			mod->duration = 999;
		}
		if (ListItr->value.HasMember("text")) {
			string text = ListItr->value["text"].GetString();
			while (text.find("VALUE") != string::npos) {
				text.replace(text.find("VALUE"), 5, to_string(std::abs(mod->value)));
			}
			while (text.find("EOTCHANGE") != string::npos) {
				text.replace(text.find("EOTCHANGE"), 9, to_string(std::abs(mod->EOTChange)));
			}
			while (text.find("DURATION") != string::npos) {
				const string turns = mod->duration > 1 ? to_string(std::abs(mod->duration)) + " turns" : to_string(std::abs(mod->duration)) + " turn";
				text.replace(text.find("DURATION"), 8, turns);
			}
			mod->text = text;
		}

		if (ListItr->value.HasMember("mText")) {
			string mtext = ListItr->value["mText"].GetString();
			while (mtext.find("VALUE") != string::npos) {
				mtext.replace(mtext.find("VALUE"), 5, to_string(std::abs(mod->value)));
			}
			while (mtext.find("EOTCHANGE") != string::npos) {
				mtext.replace(mtext.find("EOTCHANGE"), 9, to_string(std::abs(mod->EOTChange)));
			}
			while (mtext.find("DURATION") != string::npos) {
				const string turns = mod->duration > 1 ? to_string(std::abs(mod->duration)) + " turns" : to_string(std::abs(mod->duration)) + " turn";
				mtext.replace(mtext.find("DURATION"), 8, turns);
			}
			mod->mText = mtext;
		}
		if (ListItr->value.HasMember("modifier")) {
			string str = ListItr->value["modifier"].GetString();
			mod->modifier = new Modifier(*modList[str]);
			
		}
		if (ListItr->value.HasMember("filepath")) {

			string str = ListItr->value["filepath"].GetString();
			mod->filePath = str;
		}
		else {
			mod->filePath = "Textures/GUI/burnt_void.png";
		}

		string str(ListItr->name.GetString());
		modList.insert({ str,mod });
	}
	fclose(fp);
}

void DataBase::BuildEffectLists() {
	FILE* fp;
	fopen_s(&fp, "Data/Effects.json", "rb");

	char readBuffer[16384];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
	d.ParseStream(is);

	for (Value::ConstMemberIterator ListItr = d["Effects"].MemberBegin(); ListItr != d["Effects"].MemberEnd(); ListItr++) {
		EffectData* effect = new EffectData();
		effect->name = ListItr->value["name"].GetString();
		effect->value = ListItr->value["value"].GetInt();
		effect->effect = GetEffectEnum(ListItr->value["type"].GetString());
		
		if (ListItr->value.HasMember("text")) {
			string text = ListItr->value["text"].GetString();
			while (text.find("VALUE") != string::npos) {
				text.replace(text.find("VALUE"), 5, to_string(std::abs(effect->value)));
			}
			
			effect->text = text;
		}
		if (ListItr->value.HasMember("tUnit")) {
			string str = ListItr->value["tUnit"].GetString();
			effect->tUnit =  str == "TRUE";
		}
		
		if (ListItr->value.HasMember("unit")) effect->unit = ListItr->value["unit"].GetString();

		string str(ListItr->name.GetString());
		effectList.insert({ str,effect });
	}
	fclose(fp);
}

void DataBase::BuildCardListsRed() {
	FILE* fp;
	fopen_s(&fp, "Data/Red/Cards.json", "rb");

	char readBuffer[16384];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
	d.ParseStream(is);

	for (Value::ConstMemberIterator ListItr = d["Cards"].MemberBegin(); ListItr != d["Cards"].MemberEnd(); ListItr++) {
		string name = ListItr->name.GetString();
		CardData* card = new CardData();
		card->name = ListItr->value["name"].GetString();
		card->cost = ListItr->value["cost"].GetInt();
		if (ListItr->value.HasMember("modifiers")) {
			rapidjson::GenericArray<true, rapidjson::Value> arr = ListItr->value["modifiers"].GetArray();
			for (int i = 0; i < arr.Size(); i++) {
				card->modifiers.push_back(new Modifier(*modList[arr[i].GetString()]));
			}
		}
		if (ListItr->value.HasMember("filePath")) {
			string str = ListItr->value["filePath"].GetString();
			card->filePath = str;
		}
		card->cost = ListItr->value["cost"].GetInt();
		card->cType = GetCardEnum(ListItr->value["cardType"].GetString());
		if (ListItr->value.HasMember("effect")) card->effect = ListItr->value["effect"].GetString();
		if(ListItr->value.HasMember("unit")) card->unit = ListItr->value["unit"].GetString();
		card->zTag = ListItr->value["zTag"].GetString();
		card->zOTag = GetZoneOwnerEnum(ListItr->value["zOTag"].GetString());
		card->AITag = GetAITag(ListItr->value["AITag"].GetString());
		card->shaderPath = ListItr->value["shaderPath"].GetString();
		card->level = ListItr->value["level"].GetInt();
		if (card->level <= playerLevel) {
			CardListRedUnlocked.insert({ name,card });
		}
		CardListRedAll.insert({ name,card });
	}
	fclose(fp);
}

void DataBase::BuildCardListsEnemy() {
	FILE* fp;
	fopen_s(&fp, "Data/Enemy/Cards.json", "rb");

	char readBuffer[16384];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
	d.ParseStream(is);

	for (Value::ConstMemberIterator ListItr = d["Cards"].MemberBegin(); ListItr != d["Cards"].MemberEnd(); ListItr++) {
		string name = ListItr->name.GetString();
		CardData* card = new CardData();
		card->name = ListItr->value["name"].GetString();
		card->cost = ListItr->value["cost"].GetInt();

		
		if (ListItr->value.HasMember("modifiers")) {
			rapidjson::GenericArray<true, rapidjson::Value> arr = ListItr->value["modifiers"].GetArray();
			for (int i = 0; i < arr.Size(); i++) {
				card->modifiers.push_back(new Modifier(*modList[arr[i].GetString()]));
			}
		}
		if (ListItr->value.HasMember("filePath")) {
			string str = ListItr->value["filePath"].GetString();
			card->filePath = str;
		}
		card->cost = ListItr->value["cost"].GetInt();
		card->cType = GetCardEnum(ListItr->value["cardType"].GetString());
		if (ListItr->value.HasMember("effect")) card->effect = ListItr->value["effect"].GetString();
		if (ListItr->value.HasMember("unit")) card->unit = ListItr->value["unit"].GetString();
		card->zTag = ListItr->value["zTag"].GetString();
		card->zOTag = GetZoneOwnerEnum(ListItr->value["zOTag"].GetString());
		card->AITag = GetAITag(ListItr->value["AITag"].GetString());
		card->shaderPath = ListItr->value["shaderPath"].GetString();
		CardListEnemy.insert({ name,card });
	}
	fclose(fp);
}

void DataBase::BuildUnitLists() {
	FILE* fp;
	fopen_s(&fp, "Data/Units.json", "rb");

	char readBuffer[16384];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
	d.ParseStream(is);

	for (Value::ConstMemberIterator ListItr = d["Units"].MemberBegin(); ListItr != d["Units"].MemberEnd(); ListItr++) {
		string name = ListItr->name.GetString();
		UnitData* unit = new UnitData();
		unit->name= ListItr->value["name"].GetString();
		unit->physPower = ListItr->value["physPower"].GetInt();
		unit->stamina = ListItr->value["stamina"].GetInt();
		unit->filePath = ListItr->value["filePath"].GetString();

		UnitList.insert({ name,unit });
	}
	fclose(fp);
}

void DataBase::BuildEncounterLists() {
	FILE* fp;
	fopen_s(&fp, "Data/Encounters.json", "rb");

	char readBuffer[16384];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
	d.ParseStream(is);

	for (int i = 0; i < 10; i++) {
		map<string, EncounterData*> en;
		vector<string> names;
		encounters.push_back(en);
		encounterNames.push_back(names);
	}

	for (Value::ConstMemberIterator ListItr = d["Encounters"].MemberBegin(); ListItr != d["Encounters"].MemberEnd(); ListItr++) {
		string name = ListItr->name.GetString();
		EncounterData* encounter = new EncounterData();
		encounter->name = ListItr->value["name"].GetString();

		encounter->health = ListItr->value["health"].GetInt();
		encounter->armour = ListItr->value["armour"].GetInt();

		rapidjson::GenericArray<true,rapidjson::Value> arr = ListItr->value["decklist"].GetArray();
		for (int i = 0; i < arr.Size(); i++) {
			rapidjson::GenericArray<true, rapidjson::Value> ar2 = arr[i].GetArray();
			vector<string> list;
			for (int j = 0; j < ar2.Size(); j++) {
				list.push_back(ar2[j].GetString());
			}
			encounter->decklists.push_back(list);
		}
		rapidjson::GenericArray<true, rapidjson::Value> ar = ListItr->value["startingPlay"].GetArray();
		for (int i = 0; i < ar.Size(); i++) {
			encounter->startingPlay.push_back(ar[i].GetString());
		}
		encounter->level = ListItr->value["level"].GetInt();
		int level = encounter->level;
		encounters[level].insert({ name,encounter });

		encounter->actionCount = ListItr->value["actionCount"].GetInt();
		encounter->description = ListItr->value["description"].GetString();
		encounter->type = GetEncType(ListItr->value["type"].GetString());
		encounterNames[level].push_back(name);
	}
	fclose(fp);
}

void DataBase::BuildFactionLists() {
	FILE* fp;
	fopen_s(&fp, "Data/Factions.json", "rb");

	char readBuffer[16384];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
	d.ParseStream(is);

	for (Value::ConstMemberIterator ListItr = d["Factions"].MemberBegin(); ListItr != d["Factions"].MemberEnd(); ListItr++) {
		FactionData* faction = new FactionData();
		rapidjson::GenericArray<true, rapidjson::Value> arr = ListItr->value["decklist"].GetArray();
		vector<string> list;
		for (int j = 0; j < arr.Size(); j++) {
			list.push_back(arr[j].GetString());
		}
		faction->decklist = list;
		faction->name = ListItr->value["name"].GetString();
		faction->filepath = ListItr->value["filepath"].GetString();
		factionList.push_back(faction);
	}
	fclose(fp);
}

void DataBase::Init() {
	for (int i = 0; i < 11; i++) {
		string path = "Textures/GUI/ManaCosts/" + to_string(i) + ".png";
		costIcons.insert({ i,path });
	}
	retreatPhrases.push_back("Run away!");
	retreatPhrases.push_back("Fleeeeee!");
	retreatPhrases.push_back("I'm Tired");
	retreatPhrases.push_back("Not Today");
	BuildModifierLists();
	BuildCardListsRed();
	BuildCardListsEnemy();
	BuildUnitLists();
	BuildEncounterLists();
	BuildEffectLists();
	BuildFactionLists();

	rollover.loadFromFile("Sound/UI/rollover1.wav");
}