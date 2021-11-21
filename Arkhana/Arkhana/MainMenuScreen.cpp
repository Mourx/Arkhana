#include "MainMenuScreen.h"
#include "Player.h"

MainMenuScreen::MainMenuScreen(RenderTexture* w,Player* p) {
	window = w;
	player = p;
	
	type = MAIN_MENU;
	
	texFaction1.loadFromFile("Textures/Units/frog.png");
	faction1 = new GameObject();
	faction1->GetIcon()->setTexture(texFaction1);
	faction1->GetIcon()->setPosition(faction1Pos);
	faction1->GetIcon()->setScale(3, 3);
	redShader.loadFromFile("Textures/Shaders/empress.vert", Shader::Vertex);
	blueShader.loadFromFile("Textures/Shaders/magician.vert", Shader::Vertex);

	texBackground.loadFromFile("Textures/GUI/menuBackground.png");
	background.setTexture(texBackground);
	background.setPosition(0, 0);
}

void MainMenuScreen::Draw() {
	window->clear(Color::Green);
	window->draw(background);
	faction1->Draw(window);
	//window->draw(blueFactionIcon,&blueShader);
}

void MainMenuScreen::MouseMoved(Vector2f mousePos) {
	FloatRect bounds = faction1->GetIcon()->getGlobalBounds();
	if (bounds.contains(mousePos)) {
		faction1->SetHover(true);
	}
	else {
		faction1->SetHover(false);
	}
}

void MainMenuScreen::MouseClicked(Vector2f mousePos) {
	FloatRect bounds = faction1->GetIcon()->getGlobalBounds();
	if (bounds.contains(mousePos)) {
		player->SetFaction();
		nextScreen = PATH_SCREEN;
	}
	/*bounds = blueFactionIcon.getGlobalBounds();
	if (bounds.contains(mousePos)) {
		player->SetFaction();
		nextScreen = PATH_SCREEN;
	}*/
}