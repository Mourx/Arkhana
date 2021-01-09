#include <SFML/Graphics.hpp>
#include "CombatScreen.h"

using namespace sf;
using namespace std;

int main() {
	RenderWindow* window = new RenderWindow(VideoMode(960, 960), "Card games or something idk");
	CombatScreen* combat = new CombatScreen(window);
	Clock clock;
	Time elapsed;
	Event event;

	while (window->isOpen()) {
		elapsed = clock.restart();
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window->close();

			
			
		}
		window->clear(Color::Magenta);
		combat->Draw();
		window->display();
	}
	return 0;
}