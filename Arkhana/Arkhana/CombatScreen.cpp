#include "CombatScreen.h"


CombatScreen::CombatScreen(RenderWindow* w) {
	window = w;
	player = new Player(w);
	player->Setup();

	endTurn = new EndTurnButton();
}

void CombatScreen::Draw() {
	player->Draw();
}

void CombatScreen::MouseClicked(Vector2f mousePos) {
	for (Card* c : player->GetHand()) {
		FloatRect bounds = c->GetIcon().getGlobalBounds();
		if (bounds.contains(mousePos)) {
			player->selectedCard = c;
			iconOffsetX = mousePos.x - c->GetIcon().getPosition().x;
			iconOffsetY = mousePos.y - c->GetIcon().getPosition().y;
			iconSnapPos = c->GetHandPos();
		}
	}
}

void CombatScreen::MouseReleased(Vector2f mousePos) {
	if (player->selectedCard != NULL) {
		if (player->GetCurrentMana() < player->selectedCard->GetCost()) {
			player->selectedCard->GetIcon().setPosition(player->selectedCard->GetHandPos());
		}
		else {
			//player->selectedCard->Play(); 

			player->UseCard(player->selectedCard);
			//CheckDeaths();
			//UpdateStrings();
		}
		player->selectedCard = NULL;
	}
	//end turn button functionality
	FloatRect bounds = endTurn->GetIcon().getGlobalBounds();
	if (bounds.contains(mousePos)) {
		AdvanceTurn();
	}
	
}

void CombatScreen::MouseMoved(Vector2f mousePos) {
	if (!player->selectedCard == NULL) {
		player->selectedCard->GetIcon().setPosition(mousePos.x - iconOffsetX, mousePos.y - iconOffsetY);
	}
	for (Card* c : player->GetHand()) {
		FloatRect bounds = c->GetIcon().getGlobalBounds();
		if (bounds.contains(mousePos)) {
			c->SetHover(true);
		}
		else {
			c->SetHover(false);
		}
	}
}

void CombatScreen::Update(Time t) {

	player->Update(t);

}

void CombatScreen::AdvanceTurn() {

}