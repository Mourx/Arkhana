#include "ForgeScreen.h"


ForgeScreen::ForgeScreen(RenderWindow* w, DataBase* db, Player* p) {
	player = p;
	database = db;
	window = w;
	GenerateOptions();
	texBackground.loadFromFile("Textures/GUI/forgeBackground.png");
	background.setTexture(texBackground);
	font.loadFromFile("Fonts/Arial/arial.ttf");

	texPathIcon.loadFromFile("Textures/GUI/backArrow.png");
	pathIcon = new GameObject();
	pathIcon->GetIcon()->setTexture(texPathIcon);
	pathIcon->GetIcon()->setPosition(850, 900);

	texScreenShade.loadFromFile("Textures/GUI/screenShade.png");
	screenShade.setTexture(texScreenShade);
	screenShade.setPosition(0, 0);

	texUpgradeIcon.loadFromFile("Textures/GUI/forgeIcon.png");
	upgradeIcon = new GameObject();
	upgradeIcon->GetIcon()->setTexture(texUpgradeIcon);
	upgradeIcon->GetIcon()->setPosition(upgradeIconPos);
	upgradeIcon->GetIcon()->setScale(2, 2);

	texUpgAttack.loadFromFile("Textures/GUI/upgradeAttack.png");
	upgAttack = new GameObject();
	upgAttack->GetIcon()->setTexture(texUpgAttack);
	upgAttack->GetIcon()->setPosition(upgAttackPos);

	texUpgStamina.loadFromFile("Textures/GUI/upgradeStamina.png");
	upgStamina = new GameObject();
	upgStamina->GetIcon()->setTexture(texUpgStamina);
	upgStamina->GetIcon()->setPosition(upgStaminaPos);

	txtUpgradeCost.setFont(font);
	txtUpgradeCost.setPosition(txtUpgradeCostPos);
	txtUpgradeCost.setString(to_string(upgradeCost));
	FloatRect tR = txtUpgradeCost.getLocalBounds();
	txtUpgradeCost.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);

	type = FORGE_SCREEN;
}


void ForgeScreen::Draw() {
	window->draw(background);
	upgradeIcon->Draw(window);
	for (Card* c : options) {
		c->Draw(window);
		c->DrawCost(window);
	}
	
	
	window->draw(txtUpgradeCost);
	if (bUpgrading) {
		window->draw(screenShade);
		for (Card* c : player->GetDeckList()) {
			c->Draw(window);
		}
	}
	if (bSelectUpgrade) {
		window->draw(screenShade);
		upgAttack->Draw(window);
		upgStamina->Draw(window);
	}

	pathIcon->Draw(window);

}

void ForgeScreen::MouseMoved(Vector2f mousePos) {
	if (!bUpgrading) {
		FloatRect bounds = pathIcon->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			pathIcon->SetHover(true);
		}
		else {
			pathIcon->SetHover(false);
		}
		bounds = upgradeIcon->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			upgradeIcon->SetHover(true);
		}
		else {
			upgradeIcon->SetHover(false);
		}
		for (Card* c : options) {
			bounds = c->GetIcon()->getGlobalBounds();
			if (bounds.contains(mousePos)) {
				c->SetHover(true);
				selCard = c;
			}
			else {
				c->SetHover(false);
			}
		}
	}
	else if (bSelectUpgrade) {
		FloatRect bounds = upgAttack->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			upgAttack->SetHover(true);
		}
		else {
			upgAttack->SetHover(false);
		}
		bounds = upgStamina->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			upgStamina->SetHover(true);
		}
		else {
			upgStamina->SetHover(false);
		}

	}
	else if (bUpgrading) {
		for (Card* c : player->GetDeckList()) {
			FloatRect bounds = c->GetIcon()->getGlobalBounds();
			if (bounds.contains(mousePos)) {
				c->SetHover(true);
			}
			else {
				c->SetHover(false);
			}
		}
	}
	FloatRect bounds = pathIcon->GetIcon()->getGlobalBounds();
	if (bounds.contains(mousePos)) {
		pathIcon->SetHover(true);
	}
	
}

void ForgeScreen::MouseClicked(Vector2f mousePos) {
	if (bSelectUpgrade) {
		if (upgAttack->GetHover()) {
			Modifier* mod = new Modifier(*database->modList["upgrade_attack"]);
			selUpgCard->AddModifier(mod);
			selUpgCard->IncreaseStat(mod->GetStat(), mod->GetValue());
			selUpgCard = NULL;
			bUpgrading = false;
			bSelectUpgrade = false;
		}
		if (upgStamina->GetHover()) {
			Modifier* mod = new Modifier(*database->modList["upgrade_stamina"]);
			selUpgCard->AddModifier(mod);
			selUpgCard->IncreaseStat(mod->GetStat(), mod->GetValue());
			selUpgCard = NULL;
			bUpgrading = false;
			bSelectUpgrade = false;
		}
		if (pathIcon->GetHover()) {
			bSelectUpgrade = false;
		}
	}
	else if (!bUpgrading) {
		if (pathIcon->GetHover()) {
			nextScreen = PATH_SCREEN;
		}
		if (upgradeIcon->GetHover()) {
			bUpgrading = true;
			CreateDeckGrid();
		}
		if (selCard!=NULL && selCard->GetGoldCost() <= player->GetGold()) {
			player->AddCard(selCard);
			options.erase(find(options.begin(), options.end(), selCard));
			player->AddGold(-selCard->GetGoldCost());
			selCard->SetHover(false);
			selCard = NULL;
		}
	}
	else if (bUpgrading) {
		for (Card* c : player->GetDeckList()) {
			if (c->GetHover()) {
				selUpgCard = c;
				bSelectUpgrade = true;
				//subtract player gold;
				upgradeCost += 50;
			}
		}
		if (pathIcon->GetHover()) {
			bUpgrading = false;
		}
	}
	
}

void ForgeScreen::GenerateOptions() {
	selCard = NULL;
	map<string, CardData*> cardList = player->GetCardLists();

	vector<int> cIns;
	for (int i = 0; i < optionsAmount; i++) {
		int r = rand() % cardList.size();
		while (!(count(cIns.begin(), cIns.end(), r) == 0)) {
			r = rand() % cardList.size();
		}
		cIns.push_back(r);
	}
	for (int i = 0; i < cIns.size(); i++) {
		map<string, CardData*>::iterator it = cardList.begin();
		advance(it, cIns[i]);
		CardData* data = it->second;
		Card* c = new Card(*data, database);
		c->SetPosition(optionsPos[i]);

		Text t;
		t.setFont(font);
		t.setString(to_string(c->GetGoldCost()));
		
		t.setPosition(optionsPos[i] + Vector2f(75, 220));


		options.push_back(c);
		optionCostsTxt.push_back(t);
	}
}

void ForgeScreen::CreateDeckGrid() {
	int count = 0;
	for (Card* c : player->GetDeckList()) {
		if (c->GetType() == CARD_TYPE::CREATE_UNIT) {
			c->SetPosition(upgradePos + Vector2f((count % 5) * 180, (count / 5) * 210));
			count++;
		}
		else {
			c->SetPosition(Vector2f(-200, -200));
		}
	}
}

void ForgeScreen::Update(Time t) {
	player->Update(t);
}