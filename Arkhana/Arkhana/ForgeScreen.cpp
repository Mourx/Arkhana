#include "ForgeScreen.h"


ForgeScreen::ForgeScreen(RenderWindow* w, DataBase* db, Player* p) {
	player = p;
	database = db;
	window = w;
	GenerateOptions();
	texBackground.loadFromFile("Textures/GUI/forgeBackground.png");
	background.setTexture(texBackground);
	font.loadFromFile("Fonts/Arial/arial.ttf");

}


void ForgeScreen::Draw() {
	window->draw(background);
	for (Card* c : options) {
		c->Draw(window);
	}
	for (Text t : optionCostsTxt) {
		FloatRect tR = t.getLocalBounds();
		t.setOrigin(tR.left + tR.width / 2.0f, tR.top + tR.height / 2.0f);

		window->draw(t);
	}

}

void ForgeScreen::MouseMoved(Vector2f mousePos) {


}

void ForgeScreen::MouseClicked(Vector2f mousePos) {


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