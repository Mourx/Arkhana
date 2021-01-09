#include "CombatScreen.h"


CombatScreen::CombatScreen(RenderWindow* w) {
	window = w;
	player = new Player(w);
	player->Setup();
}

void CombatScreen::Draw() {
	player->Draw();
}