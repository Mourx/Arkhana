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
		faction->SetPosition(factionPosStart + Vector2f(140 * count++, 0));
		factions.push_back(faction);
	}

	
	redShader.loadFromFile("Textures/Shaders/empress.vert", Shader::Vertex);
	blueShader.loadFromFile("Textures/Shaders/magician.vert", Shader::Vertex);

	texBackground.loadFromFile("Textures/GUI/menuBackground.png");
	background.setTexture(texBackground);
	background.setPosition(0, 0);
	ShowAllCards();
}

void MainMenuScreen::Draw() {
	window->clear(Color::Green);
	window->draw(background);
	for (Faction* faction : factions) {
		faction->Draw(window);
	}
	if (bShowAllCards) {
		for (Card* c : cardGrid) {
			c->Draw(window);
		}
	}
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
}

void MainMenuScreen::ShowAllCards() {
	int count = 0;
	map<string, CardData*>::iterator it;

	for (it = database->CardListRedAll.begin(); it != database->CardListRedAll.end();it++) {
		Card* card = new Card(*it->second, database);
		card->SetPosition(Vector2f((count % 11) * 145, (count / 11) * 120));
		cardGrid.push_back(card);
		count++;
	}
}