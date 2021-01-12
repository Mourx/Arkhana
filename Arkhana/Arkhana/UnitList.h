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
using namespace rapidjson;
using namespace std;

struct UnitData {
	string name;
	int physPower;
	int magPower;
	string filePath;
};

map<UNIT_NAME,UnitData> UnitList;

UnitData goblin = { "Goblin",
					1,
					0,
					"Textures/Units/goblin.png" };

UnitData shrine = { "Shrine",
					0,
					1,
					"Textures/Units/shrine.png" };

void BuildUnitLists() {
	FILE* fp;
	fopen_s(&fp,"Data/Units.json", "rb");

	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
	d.ParseStream(is);
	
	for (Value::ConstMemberIterator itr = d["Units"].MemberBegin(); itr != d["Units"].MemberEnd();itr++) {
		cout << itr->name.GetString() << endl;
	}

	UnitList.insert({ GOBLIN,goblin });
	UnitList.insert({ SHRINE,shrine});
}