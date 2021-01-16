#include "headers.h"
#include <SFML/Graphics.hpp>
#include "CombatScreen.h"
#include "MainMenuScreen.h"
#include "RewardScreen.h"
#include "DataBase.h"
using namespace sf;
using namespace std;

int main() {
	DataBase* database = new DataBase();
	database->Init();
	RenderWindow* window = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Card games or something idk");
	CombatScreen* combat = new CombatScreen(window, database);
	MainMenuScreen* mainMenu = new MainMenuScreen(window);
	Screen* currentScreen = new Screen();
	currentScreen = combat;
	Clock clock;
	Time elapsed;
	Event event;

	while (window->isOpen()) {
		elapsed = clock.restart();
		currentScreen->Update(elapsed);
		switch (currentScreen->GetNextScreen()) {
		case MAIN_MENU:
			currentScreen = new MainMenuScreen(window);

			break;
		case COMBAT_SCREEN:
			break;
		case GAME_OVER:
			break;
		case REWARD_SCREEN:
			currentScreen = new RewardScreen(window);
			break;
		case PATH_SCREEN:
			break;
		case NONE:
			break;
		}
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed) window->close();
			Vector2f m = window->mapPixelToCoords(Mouse::getPosition(*window));
			if (event.type == Event::MouseMoved) {
				currentScreen->MouseMoved(m);
			}
			if (event.type == Event::MouseButtonPressed) {
				currentScreen->MouseClicked(m);
			}
			if (event.type == Event::MouseButtonReleased) {
				currentScreen->MouseReleased(m);
			}
			
			
		}
		window->clear(Color::Magenta);

		currentScreen->Draw();
		window->display();
	}
	return 0;
}