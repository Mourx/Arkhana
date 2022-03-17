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
	for (string s : data.effect) {
		effect.push_back(database->effectList[s]);
	}
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
	for (EffectData* data : effect) {
		txt = txt + data->text +"\n";
	}
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
		targUnit = u;
		targetZone->AddUnit(u, database);
		break;
	}
	for(EffectData* eff :effect){
		switch (eff->effect) {
		case EFFECT_TYPE::FROG_DOG:
		case EFFECT_TYPE::FEAR_FROG:
		case EFFECT_TYPE::FROGAPULT:
		case EFFECT_TYPE::CREATE_UNIT_EXHAUST:
		case EFFECT_TYPE::CREATE_UNIT_EOT:
			break;
		default:
			DoEffect(targUnit, targetZone,eff);
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
	for (Modifier* modifiers : modifiers) {
		zone->ModifyUnits(modifiers);
	}
}

void Card::UpdatePositions() {


	
}

void Card::DoEffect(Unit* targUnit, UnitZone* targZone,EffectData* eff) {
	Player* p;
	int count;
	Unit* enemy;
	Unit* unit;
	UnitZone* oppZ;
	UnitZone* zone;
	Modifier* enemyMod;
	Modifier* playerMod;
	Modifier* generalMod;
	vector<Unit*>::iterator it;
	vector<Unit*> units;
	if (targetZone == NULL) {
		targetZone = targZone;
	}
	if (eff == NULL) {
		eff = effect[0];
	}
	switch (eff->effect) {
	case EFFECT_TYPE::ARMOUR_MOD:
		p = targetZone->GetOwner();
		p->ModifyArmour(eff->value);
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
		if (enemy != NULL) database->enemy->GetZones()[(int)targetZone->GetOppositeType()]->RemoveUnit(enemy);
		break;
	case EFFECT_TYPE::POWER_RIBBIT:
		targUnit->AddModifier(modifiers[0]);
		database->enemy->DamagePhys(targUnit->GetPPower());
		break;
	case EFFECT_TYPE::CREATE_UNIT:
		for (int i = 0; i < eff->value; i++) {
			targetZone->AddUnit(new Unit(*database->UnitList[eff->unit], database->CardListAll[eff->unit]->modifiers, new Card(*database->CardListAll[eff->unit], database)), database);
		}
		break;
	case EFFECT_TYPE::CREATE_UNIT_EOT:
		for (int i = 0; i < eff->value; i++) {
			targetZone->AddUnit(new Unit(*database->UnitList[eff->unit], database->CardListAll[eff->unit]->modifiers, new Card(*database->CardListAll[eff->unit], database)), database);
		}
		break;
	case EFFECT_TYPE::CREATE_UNIT_EXHAUST:
		for (int i = 0; i < eff->value; i++) {
			targetZone->AddUnit(new Unit(*database->UnitList[eff->unit], database->CardListAll[eff->unit]->modifiers, new Card(*database->CardListAll[eff->unit], database)), database);
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
		generalMod->SetValue(eff->value);
		targetZone->GetOwner()->GetZones()[(int)ZONE_TYPE::Z_BLOCK]->AddMod(generalMod);
		generalMod = new Modifier(generalMod);
		generalMod->SetValue(eff->value);
		targetZone->GetOwner()->GetZones()[(int)ZONE_TYPE::Z_ATTACK]->AddMod(generalMod);
		break;
	case EFFECT_TYPE::ADD_ZONE_MOD:
		targetZone->AddMod(new Modifier(modifiers[0]));
		break;
	case EFFECT_TYPE::POTION_BUFF:
		targetZone->AddMod(new Modifier(*database->modList["potion_frog"]));
		break;
	case EFFECT_TYPE::MODIFY_ALL_ATTACK:
		generalMod = new Modifier(*database->modList["modify_attack_modifier"]);
		generalMod->SetValue(eff->value);
		generalMod->SetMText(eff->name + ":\n   Attack modified by " + to_string(eff->value));
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
	case EFFECT_TYPE::MODIFY_ALL:
		for (Modifier* mod : modifiers) {
			generalMod = new Modifier(mod);
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
			p->AddCardToHand(new Card(*database->CardListAll[s], database));
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
		targZone->GetOwner()->DamagePhys(eff->value);
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
		p->DamagePhys(count * eff->value);
		break;
	case EFFECT_TYPE::MODIFY_ZONE_ATTACK:
		generalMod = new Modifier(*database->modList["modify_attack_modifier"]);
		generalMod->SetValue(eff->value);
		generalMod->SetMText(eff->name + ":\n   Attack modified by " + to_string(eff->value));
		for (Unit* u : targetZone->GetUnits()) {
			u->AddModifier(generalMod);
		}
		break;
	case EFFECT_TYPE::MODIFY_ZONE_STAMINA:
		generalMod = new Modifier(*database->modList["modify_stamina_modifier"]);
		generalMod->SetValue(eff->value);
		generalMod->SetMText(eff->name + ":\n   Stamina modified by " + to_string(eff->value));
		for (Unit* u : targetZone->GetUnits()) {
			u->AddModifier(generalMod);
		}
		break;
	case EFFECT_TYPE::FROGAPULT:
		it = find(targetZone->GetUnits().begin(), targetZone->GetUnits().end(), targUnit);
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
	case EFFECT_TYPE::JUNGLE_FROG:
		for (int i = 0; i < eff->value; i++) {
			targetZone->AddUnit(new Unit(*database->UnitList[eff->unit], database->CardListAll[eff->unit]->modifiers, this), database);
		}
		count = 0;
		for (Unit* unit : targetZone->GetUnits()) {
			if (unit->GetName() == "Log") {
				count++;
			}
		}
		generalMod = new Modifier(*database->modList["modify_stamina_modifier"]);
		generalMod->SetValue(count);
		targUnit->AddModifier(generalMod);
		break;
	case EFFECT_TYPE::FIRE_FROG:
		for (int i = 0; i < eff->value; i++) {
			targetZone->AddUnit(new Unit(*database->UnitList[eff->unit], database->CardListAll[eff->unit]->modifiers, this), database);
		}
		count = 0;
		for (Unit* unit : targetZone->GetUnits()) {
			if (unit->GetName() == "Log") {
				count++;
				targetZone->RemoveUnit(unit);
			}
		}
		database->enemy->DamagePhys(count * 3);
		break;
	case EFFECT_TYPE::TWISTER:
		units = vector<Unit*>();
		targetZone = database->player->GetZones()[(int)ZONE_TYPE::Z_BLOCK];
		units = targetZone->GetUnits();
		targetZone->ClearUnits();
		targetZone = database->player->GetZones()[(int)ZONE_TYPE::Z_ATTACK];
		for (Unit* u : targetZone->GetUnits()) {
			units.push_back(u);
		}
		targetZone->ClearUnits();
		for (Unit* u : units) {
			if ((rand() % 2) == 1) {
				database->player->GetZones()[(int)ZONE_TYPE::Z_BLOCK]->AddUnit(u, database);
			}
			else {
				database->player->GetZones()[(int)ZONE_TYPE::Z_ATTACK]->AddUnit(u, database);
			}
		}
		targetZone = database->enemy->GetZones()[(int)ZONE_TYPE::Z_ATTACK];
		units = targetZone->GetUnits();
		targetZone->ClearUnits();
		targetZone = database->enemy->GetZones()[(int)ZONE_TYPE::Z_BLOCK];
		for (Unit* u : targetZone->GetUnits()) {
			units.push_back(u);
		}
		targetZone->ClearUnits();
		for (Unit* u : units) {
			if ((rand() % 2) == 1) {
				database->enemy->GetZones()[(int)ZONE_TYPE::Z_BLOCK]->AddUnit(u, database);
			}
			else {
				database->enemy->GetZones()[(int)ZONE_TYPE::Z_ATTACK]->AddUnit(u, database);
			}
		}
		break;
	case EFFECT_TYPE::GOD_FROG:
		targetZone->AddUnit(new Unit(*database->UnitList["Shrine"], database->CardListAll["Shrine"]->modifiers, new Card(*database->CardListAll["Shrine"], database)), database);
		targetZone->AddUnit(new Unit(*database->UnitList["Frog"], database->CardListAll["Frog"]->modifiers, new Card(*database->CardListAll["Frog"], database)), database);
		targetZone->AddUnit(new Unit(*database->UnitList["Frog"], database->CardListAll["Frog"]->modifiers, new Card(*database->CardListAll["Frog"], database)), database);
		targetZone->GetOwner()->Heal(5);
		targetZone->GetOwner()->ModifyArmour(5);
		break;
	case EFFECT_TYPE::MODIFY_GOLD:
		targetZone->GetOwner()->AddGold(eff->value);
		break;
	case EFFECT_TYPE::DRAW_CARD:
		targetZone->GetOwner()->DrawCards(eff->value);
		break;
	case EFFECT_TYPE::DISCARD_CARD:
		count = rand() % targetZone->GetOwner()->GetHand().size();
		targetZone->GetOwner()->Discard(count);
		break;
	case EFFECT_TYPE::MODIFY_HEALTH:
		targetZone->GetOwner()->Heal(eff->value);
		break;
	case EFFECT_TYPE::MOVE_ALLIES:
		p = targetZone->GetOwner();
		targetZone = p->GetZones()[1];
		units = vector<Unit*>();
		units = targetZone->GetUnits();
		targetZone->ClearUnits();
		targetZone = p->GetZones()[0];
		for (Unit* u : units) {
			targetZone->AddUnit(u,database);
		}
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
	for (EffectData* eff : effect) {
		if (eff->tUnit) {

			return true;
		}
	}
	return false;
}