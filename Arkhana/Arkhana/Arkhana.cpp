#include "headers.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "CombatScreen.h"
#include "MainMenuScreen.h"
#include "RewardScreen.h"
#include "PathScreen.h"
#include "ForgeScreen.h"
#include "DataBase.h"
#include "InfoPane.h"
using namespace sf;
using namespace std;

int main() {
	DataBase* database = new DataBase();
	database->Init();
	
	RenderWindow* window = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Card games or something idk",Style::Fullscreen);
	CombatScreen* combat;
	RenderTexture* screenRender = new RenderTexture();
	RenderTexture* windowRender = new RenderTexture();
	
	windowRender->create(SCREEN_WIDTH, SCREEN_HEIGHT);
	windowRender->clear();
	screenRender->create(SCREEN_WIDTH-320, SCREEN_HEIGHT-180);
	screenRender->clear();
	Player* player = new Player(screenRender, database);
	player->SetBarWindow(windowRender);
	//TAKE THIS OUT
	player->SetFaction();
	//TAKE THIS OUT
	Enemy* enemy = new Enemy();
	MainMenuScreen* mainMenu = new MainMenuScreen(screenRender,player);
	PathScreen* pathScreen = new PathScreen(screenRender, database, player);
	Screen* currentScreen = new Screen();
	InfoPane* info = new InfoPane();
	
	SoundBuffer buffer;
	//buffer.loadFromFile();
	Music music;
	
	music.openFromFile("Sound/background.wav");
	music.setVolume(10.f);
	music.setLoop(true);
	music.play();


	Sprite tempScreen;
	TRANSITION_TYPE tType = LEFT_ENTER;
	Texture preSlide;
	preSlide.create(1920, 1080);
	Texture postSlide;
	postSlide.create(1920,1080);
	Sprite slideSprite;
	Sprite staySprite;

	currentScreen = pathScreen;
	Clock clock;
	Time elapsed;
	Event event;
	Texture shaderEffect;
	shaderEffect.create(1920, 1080);
	float transitionTime = 0.6f;
	float transitionTimer = 0.0f;
	bool bTransition = false;

	Sprite render;
	Shader shader;
	bool bVortexLoaded = false;
	if (shader.loadFromFile("Textures/Shaders/vortex.frag", Shader::Fragment)) bVortexLoaded = true;


	while (window->isOpen()) {
		
		// Update Things
		elapsed = clock.restart();
		currentScreen->Update(elapsed);

		// Switch Screens if needed - do appropriate switching tasks
		switch (currentScreen->GetNextScreen()) {
		case MAIN_MENU:
			currentScreen = new MainMenuScreen(screenRender,player);
			shaderEffect.update(*window);
			preSlide.update(screenRender->getTexture());
			break;
		case COMBAT_SCREEN:
			combat = new CombatScreen(screenRender, database, player, pathScreen->GetEncounter());
			enemy = combat->GetEnemy();
			currentScreen = combat;
			bTransition = true;
			shaderEffect.update(*window);
			preSlide.update(screenRender->getTexture());
			break;
		case GAME_OVER:
			tType = DOWN_ENTER;
			break;
		case REWARD_SCREEN:
			currentScreen = new RewardScreen(screenRender,database,player,enemy);
			bTransition = true;
			shaderEffect.update(*window);
			preSlide.update(screenRender->getTexture());
			tType = DOWN_ENTER;
			break;
		case PATH_SCREEN:
			if (currentScreen->GetType() == REWARD_SCREEN) {
				pathScreen->ResetDetails(WIN);
			}
			else if (currentScreen->GetType() == FORGE_SCREEN) {
				pathScreen->ForgeVisited(true);
			}
			currentScreen = pathScreen;
			bTransition = true;
			shaderEffect.update(*window);
			preSlide.update(screenRender->getTexture());
			tType = DOWN_EXIT;
			break;
		case FORGE_SCREEN:
			pathScreen->ResetDetails(ONGOING);
			currentScreen = pathScreen->GetForge();
			bTransition = true;
			shaderEffect.update(*window);
			preSlide.update(screenRender->getTexture());
			tType = LEFT_ENTER;
			break;
		case NONE:
			break;
		}

		// Poll Mouse events 
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed) window->close();
			Vector2f m = window->mapPixelToCoords(Mouse::getPosition(*window)) + Vector2f(0,-180);
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

		// Do Transition Effects
		if (bTransition) {
			transitionTimer += elapsed.asSeconds();
			if (transitionTimer >= transitionTime) {
				bTransition = false;
				transitionTimer = 0;
			}
		}
		window->clear(Color::Magenta);
		currentScreen->SetInfo(info);
		currentScreen->Draw();
		postSlide.update(screenRender->getTexture());
		
		player->DrawPlayerBar();
		info->Draw(windowRender);

		
		windowRender->display();
		screenRender->display();
		
		tempScreen = Sprite(screenRender->getTexture());
		tempScreen.setPosition(0, 180);
		window->draw(Sprite(windowRender->getTexture()));
		window->draw(tempScreen);
		if (bTransition) {

			if (currentScreen->GetType() == COMBAT_SCREEN && bVortexLoaded) {
				transitionTime = 1.2f;
				if (transitionTimer >= transitionTime / 2.0) {
					shaderEffect.update(*window);
				}
				float timeVal = (transitionTimer >= transitionTime / 2.0) ? timeVal = transitionTime - transitionTimer : timeVal = transitionTimer;
				timeVal = timeVal / (transitionTime / 2.0);
				shader.setUniform("centre", Vector2f(0.5, 0.5));
				shader.setUniform("t", timeVal);
				shader.setUniform("iResolution", Vector2f(1920, 1080));
				shader.setUniform("currentTexture", shaderEffect);
				render.setTexture(shaderEffect);
				window->draw(render, &shader);
			}
			else {
				transitionTime = 0.6f;
				switch (tType) {
				case LEFT_ENTER:
					slideSprite.setPosition(Vector2f(-1600 + 1600 * (transitionTimer / transitionTime), 180));
					slideSprite.setTexture(postSlide);
					staySprite.setTexture(preSlide);
					staySprite.setPosition(Vector2f(0, 180));
					break;
				case DOWN_ENTER:
					slideSprite.setPosition(Vector2f(0,1080 - 900 * (transitionTimer / transitionTime)));
					slideSprite.setTexture(postSlide);
					staySprite.setTexture(preSlide);
					staySprite.setPosition(Vector2f(0, 180));
					break;
				case DOWN_EXIT:
					slideSprite.setPosition(Vector2f(0, 180 + 1080 * (transitionTimer / transitionTime)));
					slideSprite.setTexture(preSlide);
					staySprite.setTexture(postSlide);
					staySprite.setPosition(Vector2f(0, 180));
					break;
				}
				

				

				window->draw(staySprite);
				window->draw(slideSprite);
				
				
			}
		}
		window->display();
	}
	return 0;
}