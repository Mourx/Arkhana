#include "RewardScreen.h"
#include "Player.h"

RewardScreen::RewardScreen(RenderWindow* w,DataBase* db, Player* p,Enemy* e) {
	window = w;
	database = db;
	player = p;
	enemy = e;
	type = REWARD_SCREEN;
	texBackground.loadFromFile("Textures/GUI/rewardBackground.png");
	background.setTexture(texBackground);
	background.setPosition(0, 180);
	GenerateOptions();
}


void RewardScreen::GenerateOptions() {
	selCard = NULL;
	map<string, CardData*> cardList = player->GetCardLists();

	vector<int> cIns;
	for (int i = 0; i < optionsAmount; i++) {
		int r = rand() % cardList.size();
		while (!(count(cIns.begin(), cIns.end(), r)==0)) {
			r = rand() % cardList.size();
		}
		cIns.push_back(r);
	}
	for (int i = 0; i < cIns.size(); i++) {
		map<string, CardData*>::iterator it = cardList.begin();
		advance(it, cIns[i]);
		CardData* data = it->second;
		Card* c = new Card(*data, database);
		c->SetPosition(optionPos + Vector2f(i * 160, 0));
		options.push_back(c);
	}
}

void RewardScreen::Draw(){
	window->draw(background);
	for (Card* c : options) {
		c->Draw(window);
	}
}

void RewardScreen::MouseMoved(Vector2f mousePos) {
	selCard = NULL;
	for (Card* c : options) {
		FloatRect bounds = c->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			selCard = c;
		}
	}

}

void RewardScreen::MouseClicked(Vector2f mousePos) {
	if (selCard != NULL) {
		player->AddCard(selCard);
		options.clear();

		nextScreen = PATH_SCREEN;
	}
}