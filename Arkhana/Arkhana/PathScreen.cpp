#include "PathScreen.h"


PathScreen::PathScreen(RenderWindow* w, DataBase* db, Player* p) {
	window = w;
	database = db;
	player = p;
	srand(time(NULL));
	texBackground.loadFromFile("Textures/GUI/pathBackground.png");
	background.setTexture(texBackground);
	background.setPosition(0, 0);
	InitEncounters();
}

void PathScreen::Draw() {
	window->draw(background);
	for (Encounter* e : encounters) {
		e->Draw(window);
	}
}

void PathScreen::Update(Time t) {
	player->Update(t);
	Vector2f mpos = window->mapPixelToCoords(Mouse::getPosition(*window));
	for (Encounter* e : encounters) {
		FloatRect bounds = e->GetIcon()->getGlobalBounds();
		if (bounds.contains(mpos)) {
			e->SetHover(true);
		}
		else {
			e->SetHover(false);
		}
	}
}

void PathScreen::InitEncounters() {
	for (int j = 0; j < encounterAmounts.size();j++) {
		for (int i = 0; i<encounterAmounts[j]; i++) {
			Encounter* e = new Encounter(database, j);
			e->SetPosition(encPos[j] + Vector2f(rand() % 200, 250 * i));
			encounters.push_back(e);

		}
	}
}