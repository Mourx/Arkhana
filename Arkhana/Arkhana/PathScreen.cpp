#include "PathScreen.h"
#include "DataBase.h"


PathScreen::PathScreen(RenderTexture* w, Player* p) {
	window = w;
	player = p;
	srand(time(NULL));
	texBackground.loadFromFile("Textures/GUI/pathBackground.png");
	background.setTexture(texBackground);
	background.setPosition(0, 0);
	
	type = PATH_SCREEN;
	

	texForgeIcon.loadFromFile("Textures/GUI/forgeIcon.png");
	forgeIcon = new GameObject();
	forgeIcon->GetIcon()->setTexture(texForgeIcon);
	forgeIcon->GetIcon()->setPosition(forgeIconPos);
	
	description = "Clear each encounter\nto unlock the forge\nand move to the\nnext tier.";
	forgePrompt = "Visit the Forge and \nupgrade your deck!";
	forgeVisitedPrompt = "The Forge will restock\nwhen you clear this tier.";
	Init();
}

void PathScreen::Init() {
	nextScreen = NONE;
	InitEncounters();
	forge = new ForgeScreen(window, player);
	tier = 0;
}

void PathScreen::ResetDetails(COMBAT_RESULT res) {
	if (res == WIN && currentEncounter != NULL) {
		currentEncounter->SetComplete(true);
		forgeUnlocks++;
		if (forgeUnlocks >= encounterAmounts[tier]) {
			bForgeUnlocked = true;
			bForgeVisited = false;
			tier++;
			forgeUnlocks = 0;
			forge = new ForgeScreen(window, player);
		}
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
			if (bounds.contains(mousePos) && e->GetLevel() == tier && bForgeVisited == true) {
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
	if (bounds.contains(mousePos) && bForgeUnlocked == true) {
		forgeIcon->SetHover(true);
	}
	else {
		forgeIcon->SetHover(false);
	}
}

void PathScreen::MouseClicked(Vector2f mousePos) {
	if (currentEncounter != NULL) {
		switch (currentEncounter->GetType()) {
		case E_COMBAT:
		case E_ELITE:
			nextScreen = COMBAT_SCREEN;
			break;
		case E_GIFT:
			nextScreen = GIFT_SCREEN;
			break;
		case E_EVENT:
			nextScreen = EVENT_SCREEN;
		}
		
	}
	if (forgeIcon->GetHover()) {
		nextScreen = FORGE_SCREEN;
	}
}

void PathScreen::MouseReleased(Vector2f mousePos) {

}

void PathScreen::InitEncounters() {
	encounters.clear();
	for (int j = 0; j < encounterAmounts.size();j++) {
		for (int i = 0; i<encounterAmounts[j]; i++) {
			Encounter* e = new Encounter(window, j);
			e->SetPosition(encPos[j] + Vector2f(rand() % 150, 250 * i));
			encounters.push_back(e);
		}
	}
}

void PathScreen::SetInfo(InfoPane* info) {
	if (bForgeVisited == true) {
		info->SetDescription(forgeVisitedPrompt);
	}else if (bForgeUnlocked == true) {
		info->SetDescription(forgePrompt);
	}else {
		info->SetDescription(description);
	}
		
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
		info->SetHoverDescription("Will restock after \nclearing all encounters \nin a tier");
	}

}