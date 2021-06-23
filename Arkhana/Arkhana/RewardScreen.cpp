#include "RewardScreen.h"
#include "Player.h"

RewardScreen::RewardScreen(RenderTexture* w,DataBase* db, Player* p,Enemy* e) {
	window = w;
	database = db;
	player = p;
	enemy = e;
	type = REWARD_SCREEN;
	texBackground.loadFromFile("Textures/GUI/rewardBackground.png");
	background.setTexture(texBackground);
	background.setPosition(0, 0);

	texGoldIcon.loadFromFile("Textures/GUI/gold.png");
	goldIcon.setTexture(texGoldIcon);
	goldIcon.setPosition(goldIconPos);

	font.loadFromFile("Fonts/ManaSpace/manaspc.ttf");
	txtGold.setFont(font);
	txtGold.setPosition(goldTxtPos);

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
	moneyReward = (enemy->GetLevel()+1) * 22 + rand()%15;	
	txtGold.setString("+ " + to_string(moneyReward));
}

void RewardScreen::Draw(){
	window->draw(background);
	for (Card* c : options) {
		c->Draw(window);
	}
	window->draw(goldIcon);
	window->draw(txtGold);
}

void RewardScreen::MouseMoved(Vector2f mousePos) {

	selCard = NULL;
	for (Card* c : options) {
		FloatRect bounds = c->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			c->SetHover(true);
			selCard = c;
		}
		else {
			c->SetHover(false);
		}
	}

}

void RewardScreen::MouseClicked(Vector2f mousePos) {
	if (selCard != NULL) {
		player->AddCard(selCard);
		player->AddGold(moneyReward);
		options.clear();

		nextScreen = PATH_SCREEN;
	}
}

void RewardScreen::SetInfo(InfoPane* info) {

	info->SetScreenTitle("Rewards!");
	info->SetDescription("You have earned:\n-One Card\n-Gold\n\n");
	info->SetHoverTitle("");
	info->SetHoverDescription("");
	if (selCard != NULL) {
		info->SetCardInfo(selCard->GetDesc(), selCard->GetName());
	}
}