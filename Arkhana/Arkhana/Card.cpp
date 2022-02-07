#include "Card.h"
#include "Player.h"


Card::Card(CardData data,DataBase* dataB) {
	database = dataB;
	texIcon = *database->texList["Textures/Cards/CardBasic.png"];
	name = data.name;
	modifiers = data.modifiers;
	type = data.cType;
	icon.setTexture(texIcon);
	unit = data.unit;
	cost = data.cost;
	cards = data.cards;
	texCost = *database->texList["Textures/Cards/gem.png"];
	for (int i = 0; i < costPositions.size(); i++) {
		Sprite icon;
		icon.setScale(0.5, 0.5);
		icon.setTexture(texCost);
		icon.setPosition(costPositions[i]);
		costIcons.push_back(icon);
	}
	
	zTag = data.zTag;
	zType = database->GetZoneEnum(zTag);
	zOwner = data.zOTag;
	AITag = data.AITag;
	if(data.effect != "") effect = database->effectList[data.effect];
	goldCost = 80 + rand() % 32;
	SetPosition(Vector2f(300, 300));
	if (type == UNIT) {
		pPow = database->UnitList[unit]->physPower;
		stamina = database->UnitList[unit]->stamina;
	}
	cardShader.loadFromFile(data.shaderPath, Shader::Vertex);

	font = database->font;
	
	
	// load arcana symbol
	//texArcanaSymbol
	txtName.setString(name);
	txtName.setCharacterSize(24);
	txtName.setFont(font);
	txtName.setFillColor(Color::Black);

	texPhys = *database->texList["Textures/Cards/sword.png"];
	physIcon.setTexture(texPhys);

	texCardBorder = *database->texList["Textures/Cards/CardBorder.png"];
	cardBorder.setTexture(texCardBorder);
	cardBorder.setPosition(pos);
	
	FloatRect tR = physIcon.getLocalBounds();
	physIcon.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	physIcon.setPosition(pos + physPos);

	texMag = *database->texList["Textures/Cards/flame.png"];
	magIcon.setTexture(texMag);
	tR = magIcon.getLocalBounds();
	magIcon.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
	magIcon.setPosition(pos + magPos);

	for (Sprite s : costIcons) {
		tR = s.getLocalBounds();
		s.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
		s.setPosition(pos + s.getPosition());
	}

	txtPhys.setFont(database->coolFont);
	txtPhys.setCharacterSize(20);
	txtPhys.setString(to_string(pPow));
	txtPhys.setFillColor(Color::Black);
	
	txtMag.setFont(database->coolFont);
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
	if (zTag == "PASSIVE") bPassive = true;
	txtDesc.setFont(font);
	txtDesc.setString(txt);
	txtDesc.setCharacterSize(12);
	txtDesc.setFillColor(Color::Black);

	if (type == UNIT) {
		texCardArt = *database->texList[database->UnitList[unit]->filePath];
	}
	else if(data.filePath!= ""){
		texCardArt = *database->texList[data.filePath];
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
	case UNIT:
		u = new Unit(*database->UnitList[unit], modifiers, this);
		targetZone->AddUnit(u, database);
		break;
	}
	if (effect != NULL) {
		switch (effect->effect) {
		case EFFECT_TYPE::FROG_DOG:
		case EFFECT_TYPE::FEAR_FROG:
		case EFFECT_TYPE::FROGAPULT:
			break;
		default:
			DoEffect(targUnit, targetZone);
			break;
		}		
	}
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

void Card::DoEffect(Unit* targUnit, UnitZone* targZone) {
	Player* p;
	int count;
	Unit* enemy;
	Unit* unit;
	UnitZone* oppZ;
	UnitZone* zone;
	Modifier* enemyMod;
	Modifier* playerMod;
	Modifier* generalMod;
	if (targetZone == NULL) {
		targetZone = targZone;
	}
	switch (effect->effect) {
	case EFFECT_TYPE::ARMOUR_MOD:
		p = targetZone->GetOwner();
		p->ModifyArmour(effect->value);
		break;
	case EFFECT_TYPE::ZONE_MOD:
		ApplyModifier(targetZone);
		break;
	case EFFECT_TYPE::TARGET_STRONGEST:
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
		if (targUnit != NULL) ApplyModifier(targUnit);
		break;
	case EFFECT_TYPE::TARGET_UNIT:

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
	case EFFECT_TYPE::CREATE_UNIT:
		for (int i = 0; i < effect->value; i++) {
			targetZone->AddUnit(new Unit(*database->UnitList[effect->unit], modifiers, this), database);
		}
		break;
	case EFFECT_TYPE::FROG_DOG:
		targetZone = targZone;
		oppZ = database->enemy->GetZones()[(int)targetZone->GetOppositeType()];
		for (Unit* u : oppZ->GetUnits()) {
			u->AddModifier(new Modifier(*database->modList["frog_dog_modifier"]));
		}
		break;
	case EFFECT_TYPE::FEAR_FROG:
		targetZone = targZone;
		oppZ = database->enemy->GetZones()[(int)targetZone->GetOppositeType()];
		for (Unit* u : oppZ->GetUnits()) {
			u->AddModifier(new Modifier(*database->modList["fear_frog_modifier"]));
		}
		break;
	case EFFECT_TYPE::MODFIY_BOTH_BLOCK:
		enemyMod = new Modifier(modifiers[0]);
		playerMod = new Modifier(modifiers[0]);
		//playerMod->ModifyDuration(1);
		database->player->GetZones()[(int)ZONE_TYPE::Z_BLOCK]->AddMod(playerMod);
		database->enemy->GetZones()[(int)ZONE_TYPE::Z_BLOCK]->AddMod(enemyMod);
		break;
	case EFFECT_TYPE::DRIZZLE:
		generalMod = new Modifier(modifiers[0]);
		generalMod->SetValue(effect->value);
		targetZone->GetOwner()->GetZones()[(int)ZONE_TYPE::Z_BLOCK]->AddMod(generalMod);
		generalMod = new Modifier(generalMod);
		generalMod->SetValue(effect->value);
		targetZone->GetOwner()->GetZones()[(int)ZONE_TYPE::Z_ATTACK]->AddMod(generalMod);
		break;
	case EFFECT_TYPE::POTION_BUFF:
		targetZone->AddMod(new Modifier(*database->modList["potion_frog"]));
		break;
	case EFFECT_TYPE::MODIFY_ALL_ATTACK:
		generalMod = new Modifier(*database->modList["modify_attack_modifier"]);
		generalMod->SetValue(effect->value);
		generalMod->SetMText(effect->name + ":\n   Attack modified by " + to_string(effect->value));
		targetZone = database->player->GetZones()[(int)ZONE_TYPE::Z_BLOCK];
		for (Unit* u : targetZone->GetUnits()) {
			u->AddModifier(generalMod);
		}
		targetZone = database->player->GetZones()[(int)ZONE_TYPE::Z_ATTACK];
		for (Unit* u : targetZone->GetUnits()) {
			u->AddModifier(generalMod);
		}
		targetZone = database->enemy->GetZones()[(int)ZONE_TYPE::Z_ATTACK];
		for (Unit* u : targetZone->GetUnits()) {
			u->AddModifier(generalMod);
		}
		targetZone = database->enemy->GetZones()[(int)ZONE_TYPE::Z_BLOCK];
		for (Unit* u : targetZone->GetUnits()) {
			u->AddModifier(generalMod);
		}
		break;
	case EFFECT_TYPE::APPLY_MOD:
		for (Modifier* mod : modifiers) {
			generalMod = new Modifier(mod);
			targUnit->AddModifier(generalMod);
		}
		break;
	case EFFECT_TYPE::GAIN_CARD:
		for (string s : cards) {
			p = targetZone->GetOwner();
			p->AddCardToHand(new Card(*database->CardListAll[s],database));
		}
		break;
	case EFFECT_TYPE::GIVE_CARD:
		for (string s : cards) {
			p = targetZone->GetOwner();
			if (p == database->player) {
				p = database->enemy;
			}
			else {
				p = database->player;
			}
			p->AddCardToHand(new Card(*database->CardListAll[s], database));
		}
		break;
	case EFFECT_TYPE::DAMAGE:
		targZone->GetOwner()->DamagePhys(effect->value);
		break;
	case EFFECT_TYPE::CHORUS_CROAK:
		count = targZone->GetUnits().size();
		p = targetZone->GetOwner();
		if (p == database->player) {
			p = database->enemy;
		}
		else {
			p = database->player;
		}
		p->DamagePhys(count * effect->value);
		break;
	case EFFECT_TYPE::MODIFY_ZONE_ATTACK:
		generalMod = new Modifier(*database->modList["modify_attack_modifier"]);
		generalMod->SetValue(effect->value);
		generalMod->SetMText(effect->name + ":\n   Attack modified by " + to_string(effect->value));
		for (Unit* u : targetZone->GetUnits()) {
			u->AddModifier(generalMod);
		}
		break;
	case EFFECT_TYPE::MODIFY_ZONE_STAMINA:
		generalMod = new Modifier(*database->modList["modify_stamina_modifier"]);
		generalMod->SetValue(effect->value);
		generalMod->SetMText(effect->name + ":\n   Stamina modified by " + to_string(effect->value));
		for (Unit* u : targetZone->GetUnits()) {
			u->AddModifier(generalMod);
		}
		break;
	case EFFECT_TYPE::FROGAPULT:
		vector<Unit*>::iterator it = find(targetZone->GetUnits().begin(), targetZone->GetUnits().end(), targUnit);
		if (it != targetZone->GetUnits().end()) {
			it++;
			unit = *it;
			it = targetZone->GetUnits().end();
			count = (unit)->GetPPower();
			targetZone->RemoveUnit(unit);
			p = targetZone->GetOwner();
			if (p == database->player) {
				p = database->enemy;
			}
			else {
				p = database->player;
			}
			p->DamagePhys(count);
		}
		break;
	}
}

void Card::SetCostChange(int change) {
	costChange = change;


}

void Card::Draw(RenderTexture* w) {
	w->draw(icon, &cardShader);
	if (type == UNIT) {
		w->draw(txtPhys);
		w->draw(txtMag);
		w->draw(physIcon);
		w->draw(magIcon);
	}
	for (int i = 0; i < GetCost(); i++) {
		w->draw(costIcons[i]);
	}
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

	for (int i = 0; i < costIcons.size(); i++) {
		tR = costIcons[i].getLocalBounds();
		costIcons[i].setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);
		costIcons[i].setPosition(pos + costPositions[i]);
	}

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

bool Card::IsUnitTarget() {
	if (effect!=nullptr && effect->tUnit) {

		return true;
	}else{
		return false;
	}
}