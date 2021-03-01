#include "PathScreen.h"


PathScreen::PathScreen(RenderWindow* w, DataBase* db, Player* p) {
	window = w;
	database = db;
	player = p;
	srand(time(NULL));
	texBackground.loadFromFile("Textures/GUI/pathBackground.png");
	background.setTexture(texBackground);
	background.setPosition(0, 180);
	InitEncounters();
}

void PathScreen::ResetDetails(COMBAT_RESULT res) {
	if (res == WIN) {
		currentEncounter->SetComplete(true);
	}
	currentEncounter = NULL;
	nextScreen = NONE;
}

void PathScreen::Draw() {
	window->draw(background);
	for (Encounter* e : encounters) {
		e->Draw(window);
	}
}

void PathScreen::Update(Time t) {
	player->Update(t);
	
}

void PathScreen::MouseMoved(Vector2f mousePos) {
	
	bool bAny = false;
	for (Encounter* e : encounters) {
		if (e->GetComplete() == false) {
			FloatRect bounds = e->GetIcon()->getGlobalBounds();
			if (bounds.contains(mousePos)) {
				e->SetHover(true);
				currentEncounter = e;
				bAny = true;
			}
			else {
				e->SetHover(false);
			}
		}
	}
	if (bAny == false) {
		currentEncounter = NULL;
	}
}

void PathScreen::MouseClicked(Vector2f mousePos) {
	if (currentEncounter != NULL) {
		nextScreen = COMBAT_SCREEN;
	}
}

void PathScreen::MouseReleased(Vector2f mousePos) {

}

void PathScreen::InitEncounters() {
	for (int j = 0; j < encounterAmounts.size();j++) {
		for (int i = 0; i<encounterAmounts[j]; i++) {
			Encounter* e = new Encounter(window,database, j);
			e->SetPosition(encPos[j] + Vector2f(rand() % 200, 250 * i));
			encounters.push_back(e);

		}
	}
}