#include "Card.h"


Card::Card(CardData data,DataBase* dataB) {
	database = dataB;
	texIcon.loadFromFile("Textures/Cards/CardBasic.png");
	name = data.name;
	modifiers = data.modifiers;
	type = data.cType;
	icon.setTexture(texIcon);
	unit = data.unit;
	SetPosition(Vector2f(300, 300));
	if (type == UNIT) {
		pPow = database->UnitList[name]->physPower;
		mPow = database->UnitList[name]->magPower;
	}
	cardShader.loadFromFile(data.shaderPath, Shader::Vertex);

	font.loadFromFile("Fonts/Arial/arial.ttf");
	
	// load arcana symbol
	//texArcanaSymbol
	txtName.setString(name);
	txtName.setCharacterSize(20);
	txtName.setFont(font);
	txtName.setFillColor(Color::Black);

	texPhys.loadFromFile("Textures/Cards/sword.png");
	physIcon.setTexture(texPhys);
	
	FloatRect tR = physIcon.getLocalBounds();
	physIcon.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	physIcon.setPosition(pos + physPos);

	texMag.loadFromFile("Textures/Cards/flame.png");
	magIcon.setTexture(texMag);
	tR = magIcon.getLocalBounds();
	magIcon.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	magIcon.setPosition(pos + magPos);

	txtPhys.setFont(font);
	txtPhys.setCharacterSize(20);
	txtPhys.setString(to_string(pPow));
	txtPhys.setFillColor(Color::Black);
	
	txtMag.setFont(font);
	txtMag.setCharacterSize(20);
	txtMag.setString(to_string(mPow));
	txtMag.setFillColor(Color::Black);

	string txt;
	for (Modifier* m : modifiers) {
		txt = txt +"\n"+ m->GetText();
	}
	txtDesc.setFont(font);
	txtDesc.setString(txt);
	txtDesc.setCharacterSize(12);
	txtDesc.setFillColor(Color::Black);
	
}

void Card::Play(UnitZone* zone) {
	Unit* u;
	switch (type) {
	case UNIT:
		u = new Unit(*database->UnitList[unit], modifiers);
		zone->AddUnit(u);
		
		break;
	case SPELL:
		ApplyModifier(zone);
		break;
	}
}

void Card::ApplyModifier(UnitZone* zone) {
	for (Modifier* modifier : modifiers) {
		mod = modifier->GetModType();
		switch (mod) {
		case MODIFIER_TYPE::ZONE_MOD:
			zone->ModifyUnits(modifier);
			break;
		case MODIFIER_TYPE::UNIT_MOD:
			break;
		case MODIFIER_TYPE::INSTANT_MOD:
			break;
		}
	}
}

void Card::UpdatePositions() {


	
}

void Card::Draw(RenderWindow* w) {
	w->draw(icon, &cardShader);
	if (type == UNIT) {
		w->draw(txtPhys);
		w->draw(txtMag);
		w->draw(physIcon);
		w->draw(magIcon);
	}
	w->draw(txtName);
	w->draw(txtDesc);
}

void Card::SetPosition(Vector2f p) {
	pos = p;
	GameObject::SetPosition(pos);
	
	physIcon.setPosition(pos+physPos);

	magIcon.setPosition(pos+magPos);


	FloatRect tR = txtPhys.getLocalBounds();
	txtPhys.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtPhys.setPosition(pos + txtPhysPos);

	tR = txtMag.getLocalBounds();
	txtMag.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtMag.setPosition(pos + txtMagPos);

	tR = txtName.getLocalBounds();
	txtName.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtName.setPosition(pos + txtNamePos);

	tR = txtDesc.getLocalBounds();
	txtDesc.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtDesc.setPosition(pos + txtDescPos);
	
}