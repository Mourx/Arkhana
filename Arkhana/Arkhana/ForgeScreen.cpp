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
}


void ForgeScreen::Draw() {
	window->draw(background);
	pathIcon->Draw(window);
	upgradeIcon->Draw(window);
	for (Card* c : options) {
		c->Draw(window);
	}
	for (Text t : optionCostsTxt) {
		FloatRect tR = t.getLocalBounds();
		t.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);

		window->draw(t);
	}
	if (bUpgrading) {
		window->draw(screenShade);
		for (Card* c : player->GetDeckList()) {
			c->Draw(window);
		}
	}

}

void ForgeScreen::MouseMoved(Vector2f mousePos) {

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
}

void ForgeScreen::MouseClicked(Vector2f mousePos) {

	if (pathIcon->GetHover()) {
		nextScreen = PATH_SCREEN;
	}
	if (upgradeIcon->GetHover()) {
		bUpgrading = true;
		CreateDeckGrid();
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
		c->SetPosition(upgradePos + Vector2f((count%5) * 180,(count/5)*210));
		count++;
	}
}