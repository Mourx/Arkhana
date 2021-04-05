#include "headers.h"
#include <SFML/Graphics.hpp>
#include "CombatScreen.h"
#include "MainMenuScreen.h"
#include "RewardScreen.h"
#include "PathScreen.h"
#include "ForgeScreen.h"
#include "DataBase.h"
using namespace sf;
using namespace std;

int main() {
	DataBase* database = new DataBase();
	database->Init();
	
	RenderWindow* window = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Card games or something idk",Style::Fullscreen);
	CombatScreen* combat;
	Player* player = new Player(window, database);
	//TAKE THIS OUT
	player->SetFaction();
	//TAKE THIS OUT
	Enemy* enemy = new Enemy();
	MainMenuScreen* mainMenu = new MainMenuScreen(window,player);
	PathScreen* pathScreen = new PathScreen(window, database, player);
	Screen* currentScreen = new Screen();

	currentScreen = pathScreen;
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
			combat = new CombatScreen(window, database, player, pathScreen->GetEncounter());
			enemy = combat->GetEnemy();
			currentScreen = combat;
			break;
		case GAME_OVER:
			break;
		case REWARD_SCREEN:
			currentScreen = new RewardScreen(window,database,player,enemy);
			break;
		case PATH_SCREEN:

			pathScreen->ResetDetails(WIN);
			currentScreen = pathScreen;
			break;
		case FORGE_SCREEN:
			pathScreen->ResetDetails(ONGOING);
			currentScreen = pathScreen->GetForge();
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
		if (currentScreen->GetType() != MAIN_MENU) player->DrawPlayerBar();
		window->display();
	}
	return 0;
}