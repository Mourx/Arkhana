#include "GiftScreen.h"


GiftScreen::GiftScreen(RenderTexture* w, DataBase* db, Player* p) {
	window = w;

	SetupGift();
}

void GiftScreen::Draw() {
	window->clear(Color::Green);
	window->draw(background);
}



void GiftScreen::MouseMoved(Vector2f mousePos) {

}

void GiftScreen::MouseClicked(Vector2f mousePos) {

}

void GiftScreen::SetupGift() {
	_cardGift = new Card(*database->CardListRedAll[database->GetRandomCard(UNCOMMON)], database);
}