#include "RewardScreen.h"
#include "Player.h"

RewardScreen::RewardScreen(RenderWindow* w,DataBase* db, Player* p) {
	window = w;
	database = db;
	player = p;

	texBackground.loadFromFile("Textures/GUI/rewardBackground.png");
	background.setTexture(texBackground);
	background.setPosition(0, 0);
	GenerateOptions();
}


void RewardScreen::GenerateOptions() {
	map<string,CardData*> cardList = player->GetCardLists();
	for (int i = 0; i < optionsAmount; i++) {
		map<string, CardData*>::iterator it = cardList.begin();
		advance(it, rand() % cardList.size());
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
	player->AddCard(selCard);
	options.clear();

	nextScreen = PATH_SCREEN;
}