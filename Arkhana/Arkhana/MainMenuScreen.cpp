#include "MainMenuScreen.h"
#include "Player.h"

MainMenuScreen::MainMenuScreen(RenderTexture* w,Player* p) {
	window = w;
	player = p;
	
	type = MAIN_MENU;
	database = player->GetDatabase();
	int count = 0;
	for (FactionData* data : database->factionList) {
		Faction* faction = new Faction(player->GetDatabase(),data);
		faction->SetPosition(factionPosStart + Vector2f(75 * count++, 0));
		factions.push_back(faction);
	}

	
	redShader.loadFromFile("Textures/Shaders/empress.vert", Shader::Vertex);
	blueShader.loadFromFile("Textures/Shaders/magician.vert", Shader::Vertex);

	texBackground.loadFromFile("Textures/GUI/menuBackground.png");
	background.setTexture(texBackground);
	background.setPosition(0, 0);
}

void MainMenuScreen::Draw() {
	window->clear(Color::Green);
	window->draw(background);
	for (Faction* faction : factions) {
		faction->Draw(window);
	}
	//window->draw(blueFactionIcon,&blueShader);
}

void MainMenuScreen::MouseMoved(Vector2f mousePos) {
	for (Faction* faction : factions) {
		FloatRect bounds = faction->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			faction->SetHover(true);
		}
		else {
			faction->SetHover(false);
		}
	}
}

void MainMenuScreen::MouseClicked(Vector2f mousePos) {
	for (Faction* faction : factions) {
		FloatRect bounds = faction->GetIcon()->getGlobalBounds();
		if (bounds.contains(mousePos)) {
			player->SetFaction(faction);
			nextScreen = PATH_SCREEN;
		}
	}
	/*bounds = blueFactionIcon.getGlobalBounds();
	if (bounds.contains(mousePos)) {
		player->SetFaction();
		nextScreen = PATH_SCREEN;
	}*/
}