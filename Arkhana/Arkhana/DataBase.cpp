#include "DataBase.h"
#include "Modifier.h"
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
		if (ListItr->value.HasMember("text")) {
			string text = ListItr->value["text"].GetString();
			while (text.find("VALUE") != string::npos) {
				text.replace(text.find("VALUE"), 5, to_string(mod->value));
			}
			mod->text = text;
		}
		string str(ListItr->name.GetString());
		modList.insert({ str,mod });
	}
	
}

void DataBase::BuildCardLists() {
	FILE* fp;
	fopen_s(&fp, "Data/Empress/Cards.json", "rb");

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
			for (Value::ConstMemberIterator modifiersItr = ListItr->value["modifiers"].MemberBegin(); modifiersItr != ListItr->value["modifiers"].MemberEnd(); modifiersItr++) {
				string str = modifiersItr->value.GetString();
				card->modifiers.push_back(new Modifier(*modList[str]));
			}
		}
		if (ListItr->value.HasMember("pPow")) {
			card->pPow = ListItr->value["pPow"].GetInt();
		}
		if (ListItr->value.HasMember("mPow")) {
			card->mPow = ListItr->value["mPow"].GetInt();
		}
		card->cost = ListItr->value["cost"].GetInt();
		card->cType = GetCardEnum(ListItr->value["cardType"].GetString());
	
		if(ListItr->value.HasMember("unit")) card->unit = ListItr->value["unit"].GetString();
		card->filePath = ListItr->value["filePath"].GetString();
		card->shaderPath = ListItr->value["shaderPath"].GetString();
		CardList.insert({ name,card });
	}

}

void DataBase::BuildUnitLists() {
	FILE* fp;
	fopen_s(&fp, "Data/Empress/Units.json", "rb");

	char readBuffer[16384];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
	d.ParseStream(is);

	for (Value::ConstMemberIterator ListItr = d["Units"].MemberBegin(); ListItr != d["Units"].MemberEnd(); ListItr++) {
		string name = ListItr->name.GetString();
		UnitData* unit = new UnitData();
		unit->name= ListItr->value["name"].GetString();
		unit->physPower = ListItr->value["physPower"].GetInt();
		unit->magPower = ListItr->value["magPower"].GetInt();
		unit->filePath = ListItr->value["filePath"].GetString();

		UnitList.insert({ name,unit });
	}
}