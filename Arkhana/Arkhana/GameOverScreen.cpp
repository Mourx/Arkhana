#include "GameOverScreen.h"

GameOverScreen::GameOverScreen(RenderTexture* w, DataBase* db, Player* p) {
	window = w;
	database = db;
	player = p;
	texBackground.loadFromFile("Textures/GUI/pathBackground.png");
	background.setTexture(texBackground);
	background.setPosition(0, 0);
	mainMenuButton = new GameObject();
	texMenuButton.loadFromFile("Textures/GUI/menuButton.png");
	mainMenuButton->GetIcon()->setTexture(texMenuButton);
	mainMenuButton->GetIcon()->setOrigin(Vector2f(mainMenuButton->GetIcon()->getLocalBounds().width / 2, mainMenuButton->GetIcon()->getLocalBounds().height / 2));
	mainMenuButton->SetPosition(Vector2f(window->getSize().x/2, window->getSize().y/2));
}

void GameOverScreen::Draw() {
	window->draw(background);
	mainMenuButton->Draw(window);
}


void GameOverScreen::Update(Time t) {

}


void GameOverScreen::MouseMoved(Vector2f mousePos) {
	FloatRect bounds = mainMenuButton->GetIcon()->getGlobalBounds();
	if (bounds.contains(mousePos)) {
		mainMenuButton->SetHover(true);
	}
}

void GameOverScreen::MouseClicked(Vector2f mousePos) {
	FloatRect bounds = mainMenuButton->GetIcon()->getGlobalBounds();
	if (bounds.contains(mousePos)) {
		nextScreen = MAIN_MENU;
	}
}