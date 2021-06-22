#include "MainMenuScreen.h"
#include "Player.h"

MainMenuScreen::MainMenuScreen(RenderTexture* w,Player* p) {
	window = w;
	player = p;
	
	type = MAIN_MENU;
	texBlueFaction.loadFromFile("Textures/Cards/CardBasic.png");
	texRedFaction.loadFromFile("Textures/Cards/CardBasic.png");

	redFactionIcon.setTexture(texRedFaction);
	redFactionIcon.setPosition(redFactionPos);
	blueFactionIcon.setTexture(texBlueFaction);
	blueFactionIcon.setPosition(blueFactionPos);

	redShader.loadFromFile("Textures/Shaders/empress.vert", Shader::Vertex);
	blueShader.loadFromFile("Textures/Shaders/magician.vert", Shader::Vertex);
}

void MainMenuScreen::Draw() {
	window->draw(redFactionIcon,&redShader);
	window->draw(blueFactionIcon,&blueShader);
}

void MainMenuScreen::MouseMoved(Vector2f mousePos) {

}

void MainMenuScreen::MouseClicked(Vector2f mousePos) {
	FloatRect bounds = redFactionIcon.getGlobalBounds();
	if (bounds.contains(mousePos)) {
		player->SetFaction();
		nextScreen = PATH_SCREEN;
	}
	bounds = blueFactionIcon.getGlobalBounds();
	if (bounds.contains(mousePos)) {
		player->SetFaction();
		nextScreen = PATH_SCREEN;
	}
}