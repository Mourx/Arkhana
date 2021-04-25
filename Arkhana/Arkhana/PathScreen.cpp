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
	type = PATH_SCREEN;
	forge = new ForgeScreen(window, database, player);

	texForgeIcon.loadFromFile("Textures/GUI/forgeIcon.png");
	forgeIcon = new GameObject();
	forgeIcon->GetIcon()->setTexture(texForgeIcon);
	forgeIcon->GetIcon()->setPosition(forgeIconPos);

	description = "Clear each encounter\nto unlock the forge\nand move to the\nnext tier.";
}

void PathScreen::ResetDetails(COMBAT_RESULT res) {
	if (res == WIN && currentEncounter != NULL) {
		currentEncounter->SetComplete(true);
	}
	currentEncounter = NULL;
	nextScreen = NONE;
	forge->ResetNextScreen();
}

void PathScreen::Draw() {
	window->draw(background);
	forgeIcon->Draw(window);
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
	FloatRect bounds = forgeIcon->GetIcon()->getGlobalBounds();
	if (bounds.contains(mousePos)) {
		forgeIcon->SetHover(true);
	}
	else {
		forgeIcon->SetHover(false);
	}
}

void PathScreen::MouseClicked(Vector2f mousePos) {
	if (currentEncounter != NULL) {
		nextScreen = COMBAT_SCREEN;
	}
	if (forgeIcon->GetHover()) {
		nextScreen = FORGE_SCREEN;
	}
}

void PathScreen::MouseReleased(Vector2f mousePos) {

}

void PathScreen::InitEncounters() {
	for (int j = 0; j < encounterAmounts.size();j++) {
		for (int i = 0; i<encounterAmounts[j]; i++) {
			Encounter* e = new Encounter(window,database, j);
			e->SetPosition(encPos[j] + Vector2f(rand() % 150, 250 * i));
			encounters.push_back(e);

		}
	}
}

void PathScreen::SetInfo(InfoPane* info) {
	info->SetDescription(description);
	info->SetScreenTitle("Select Encounter");

	
	if (currentEncounter != NULL) {
		info->SetHoverDescription(currentEncounter->GetTypeDesc());
		info->SetHoverTitle(currentEncounter->GetTypeTitle());
	}
	else {
		info->SetHoverTitle("");
		info->SetHoverDescription("");
	}
	if (forgeIcon->GetHover()) {
		info->SetHoverTitle("Forge");
		info->SetHoverDescription("Available after clearing\nall encounters in\na tier");
	}

}