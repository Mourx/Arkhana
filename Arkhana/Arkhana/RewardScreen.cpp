#include "RewardScreen.h"
#include "Player.h"
#include "DataBase.h"

RewardScreen::RewardScreen(RenderTexture* w, Player* p,Enemy* e) {
	window = w;
	player = p;
	enemy = e;
	type = REWARD_SCREEN;
	texBackground.loadFromFile("Textures/GUI/rewardBackground.png");
	background.setTexture(texBackground);
	background.setPosition(0, 0);

	texGoldIcon.loadFromFile("Textures/GUI/gold.png");
	goldIcon.setTexture(texGoldIcon);
	goldIcon.setPosition(goldIconPos);

	txtGold.setFont(database->coolFont);
	txtGold.setPosition(goldTxtPos);

	GenerateOptions();
}


void RewardScreen::GenerateOptions() {
	selCard = NULL;
	
	map<string, CardData*> cardList = player->GetCardLists();

	vector<int> cIns;
	for (int i = 0; i < optionsAmount; i++) {
		int random = rand() % 100;
		RARITY rarity = COMMON;
		if (random < 10) {
			rarity = RARE;
		}
		else if (random < 40) {
			rarity = UNCOMMON;
		}
		CardData* data = cardList[database->GetRandomCard(rarity)];
		Card* c = new Card(*data);
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
		info->SetCardInfo(selCard);
	}
}