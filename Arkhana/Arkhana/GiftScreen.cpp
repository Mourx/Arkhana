#include "GiftScreen.h"
#include "DataBase.h"
#include "Player.h"

GiftScreen::GiftScreen(RenderTexture* w, Player* p) {
	window = w;
	player = p;
	SetupGift();
	nextButton = new GameObject();
	nextButton->SetPosition(Vector2f(400, 600));
	texButton.loadFromFile("Textures/GUI/nextButton.png");
	nextButton->GetIcon()->setTexture(texButton);
	type = GIFT_SCREEN;
}

void GiftScreen::Draw() {
	window->clear(Color::Green);
	window->draw(background);
	if(_cardGift!=NULL)_cardGift->Draw(window);
	nextButton->Draw(window);
}


void GiftScreen::MouseMoved(Vector2f mousePos) {
	
}

void GiftScreen::MouseClicked(Vector2f mousePos) {
	if (nextButton->GetIcon()->getGlobalBounds().contains(mousePos)) {
		nextScreen = PATH_SCREEN;
	}
	if (_cardGift != NULL && _cardGift->GetIcon()->getGlobalBounds().contains(mousePos)) {
		player->AddCard(_cardGift);
		_cardGift = NULL;
	}
}

void GiftScreen::SetupGift() {
	_cardGift = new Card(*database->CardListRedAll[database->GetRandomCard(UNCOMMON)]);
	_cardGift->SetPosition(Vector2f(300, 300));
}