#include "headers.h"
#include <SFML/Graphics.hpp>
#include "CombatScreen.h"
#include "MainMenuScreen.h"
#include "RewardScreen.h"
#include "PathScreen.h"
#include "DataBase.h"
using namespace sf;
using namespace std;

int main() {
	DataBase* database = new DataBase();
	database->Init();
	
	RenderWindow* window = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Card games or something idk",Style::Fullscreen);
	
	Player* player = new Player(window, database);
	MainMenuScreen* mainMenu = new MainMenuScreen(window,player);
	PathScreen* pathScreen = new PathScreen(window, database, player);
	Screen* currentScreen = new Screen();
	currentScreen = mainMenu;
	Clock clock;
	Time elapsed;
	Event event;

	while (window->isOpen()) {
		elapsed = clock.restart();
		currentScreen->Update(elapsed);
		switch (currentScreen->GetNextScreen()) {
		case MAIN_MENU:
			currentScreen = new MainMenuScreen(window,player);

			break;
		case COMBAT_SCREEN:
			currentScreen = new CombatScreen(window, database, player, pathScreen->GetEncounter());
			break;
		case GAME_OVER:
			break;
		case REWARD_SCREEN:
			currentScreen = new RewardScreen(window,database,player);
			break;
		case PATH_SCREEN:
			pathScreen->ResetDetails(WIN);
			currentScreen = pathScreen;
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
		if(currentScreen->GetType() != MAIN_MENU) player->DrawPlayerBar();
		currentScreen->Draw();
		window->display();
	}
	return 0;
}