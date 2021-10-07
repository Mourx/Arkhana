#include "Card.h"
#include "Player.h"


Card::Card(CardData data,DataBase* dataB) {
	database = dataB;
	texIcon.loadFromFile("Textures/Cards/CardBasic.png");
	name = data.name;
	modifiers = data.modifiers;
	type = data.cType;
	icon.setTexture(texIcon);
	unit = data.unit;
	cost = data.cost;
	texCost.loadFromFile(database->costIcons[cost]);
	zTag = data.zTag;
	zType = database->GetZoneEnum(zTag);
	zOwner = data.zOTag;
	AITag = data.AITag;
	if(data.effect != "") effect = database->effectList[data.effect];
	goldCost = 80 + rand() % 32;
	costIcon.setTexture(texCost);
	SetPosition(Vector2f(300, 300));
	if (type == CREATE_UNIT) {
		pPow = database->UnitList[unit]->physPower;
		stamina = database->UnitList[unit]->stamina;
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

	texCardBorder.loadFromFile("Textures/Cards/CardBorder.png");
	cardBorder.setTexture(texCardBorder);
	cardBorder.setPosition(pos);
	
	FloatRect tR = physIcon.getLocalBounds();
	physIcon.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	physIcon.setPosition(pos + physPos);

	texMag.loadFromFile("Textures/Cards/flame.png");
	magIcon.setTexture(texMag);
	tR = magIcon.getLocalBounds();
	magIcon.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	magIcon.setPosition(pos + magPos);

	tR = costIcon.getLocalBounds();
	costIcon.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	costIcon.setPosition(pos + costIconPos);


	txtPhys.setFont(font);
	txtPhys.setCharacterSize(20);
	txtPhys.setString(to_string(pPow));
	txtPhys.setFillColor(Color::Black);
	
	txtMag.setFont(font);
	txtMag.setCharacterSize(20);
	txtMag.setString(to_string(stamina));
	txtMag.setFillColor(Color::Black);

	string txt;
	if(effect != NULL) txt = txt + effect->text;
	for (Modifier* m : modifiers) {
		txt = txt + m->GetText() + "\n";
	}
	if (txt == "") txt = zTag;
	else txt = txt + "\n" + zTag;
	if (zTag == "UNDERCOVER") bUndercover = true;
	txtDesc.setFont(font);
	txtDesc.setString(txt);
	txtDesc.setCharacterSize(12);
	txtDesc.setFillColor(Color::Black);

	if (type == CREATE_UNIT) {
		texCardArt.loadFromFile(database->UnitList[unit]->filePath);
	}
	else if(data.filePath!= ""){
		texCardArt.loadFromFile(data.filePath);
	}
	cardArt.setTexture(texCardArt);
	cardArt.setScale(2, 2);
	
	txtGoldCost.setFont(font);

}

void Card::UpdateStrings() {
	txtPhys.setString(to_string(pPow));
	txtMag.setString(to_string(stamina));

}

void Card::Play(Unit* targUnit) {
	Unit* u;
	switch (type) {
	case CREATE_UNIT:
		u = new Unit(*database->UnitList[unit], modifiers,this);
		targetZone->AddUnit(u,database);
		break;
	case APPLY_ZONE_MOD:
		ApplyModifier(targetZone);
		break;
	case TARGET_STRONGEST:
		targUnit = NULL;
		for (Unit* u : targetZone->GetUnits()) {
			if (targUnit == NULL) targUnit = u;
			else if (targUnit->GetPPower() < u->GetPPower()) {
				targUnit = u;
			}
			else if (targUnit->GetPPower() == u->GetPPower()) {
				if (targUnit->GetStamina() < u->GetStamina()) {
					targUnit = u;
				}
			}
		
		}

		if(targUnit != NULL) ApplyModifier(targUnit);
		break;
	}
	if(effect != NULL) DoEffect(targUnit);
	bHasTargetZone = false;
	targetZone->CheckStamina();
	targetZone = NULL;
}

void Card::Play(UnitZone* zone,Unit* targUnit) {
	targetZone = zone;
	Play(targUnit);
}


void Card::ApplyModifier(Unit* u) {
	for (Modifier* m : modifiers) {
		u->AddModifier(m);
	}
}

void Card::ApplyModifier(UnitZone* zone) {
	for (Modifier* modifier : modifiers) {
		zone->ModifyUnits(modifier);
	}
}

void Card::UpdatePositions() {


	
}

void Card::DoEffect(Unit* targUnit) {
	Player* p;
	int count;
	Unit* enemy;
	switch (effect->effect) {
	case EFFECT_TYPE::ARMOUR_MOD:
		p = targetZone->GetOwner();
		p->ModifyArmour(effect->value);
		break;
	case EFFECT_TYPE::FROG_FRIENDS:
		count = 0;
		for (Unit* u : targetZone->GetUnits()) {
			if (u == targUnit) {
				break;
			}
			count++;
		}
		targetZone->RemoveUnit(targUnit);
		
		enemy = NULL;
		while (enemy == NULL && count >= 0) {
			if (database->enemy->GetZones()[(int)targetZone->GetOppositeType()]->GetUnits().size() > count) {
				enemy = database->enemy->GetZones()[(int)targetZone->GetOppositeType()]->GetUnits()[count];
				break;
			}
			count--;
		}	
		if(enemy != NULL) database->enemy->GetZones()[(int)targetZone->GetOppositeType()]->RemoveUnit(enemy);
		break;
	case EFFECT_TYPE::POWER_RIBBIT:
		targUnit->AddModifier(modifiers[0]);
		database->enemy->DamagePhys(targUnit->GetPPower());
		break;
	case EFFECT_TYPE::LOG_FROG:
		for (int i = 0; i < effect->value; i++) {
			targetZone->AddUnit(new Unit(*database->UnitList["Log"], modifiers, this), database);
		}
		
		break;
	}
}

void Card::SetCostChange(int change) {
	costChange = change;
	texCost.loadFromFile(database->costIcons[cost+costChange]);
	costIcon.setTexture(texCost);

}

void Card::Draw(RenderTexture* w) {
	w->draw(icon, &cardShader);
	if (type == CREATE_UNIT) {
		w->draw(txtPhys);
		w->draw(txtMag);
		w->draw(physIcon);
		w->draw(magIcon);
	}
	w->draw(costIcon);
	w->draw(txtName);
	//w->draw(txtDesc);
	w->draw(cardArt);
	if (bHover) {
		w->draw(cardBorder);
	}
}

void Card::DrawCost(RenderTexture* w) {
	w->draw(txtGoldCost);
}

void Card::SetPosition(Vector2f p) {
	pos = p;
	GameObject::SetPosition(pos);
	
	physIcon.setPosition(pos+physPos);

	magIcon.setPosition(pos+magPos);
	cardBorder.setPosition(pos);

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

	tR = costIcon.getLocalBounds();
	costIcon.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	costIcon.setPosition(pos + costIconPos);

	cardArt.setPosition(pos + cardArtPos);


	txtGoldCost.setString(to_string(GetGoldCost()));
	tR = txtGoldCost.getLocalBounds();
	txtGoldCost.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	txtGoldCost.setPosition(pos + goldCostPos);
}

void Card::SetHover(bool b){
	if (bHover != b && b == true) {


		database->sound.setBuffer(database->rollover);
		database->sound.play();
	}
	bHover = b;
		
}