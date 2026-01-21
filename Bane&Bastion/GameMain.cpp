#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Config.h"
#include "Game.h"
#include <memory>

//-----------------------------------------------------------------------------------------------------------
int main()
{
	using namespace Bane_Bastion;
	bool isKeysPressed = false;
	sf::Keyboard::Key pressedKey = sf::Keyboard::Unknown;
	State currentState = State::ExitGame;
	int seed = (int)time(nullptr);
	srand(seed);
	sf::Event event;

	// Init window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Bane_Bastion!");

	Game::GetInstance().SetWindowRef(&window);
	// Game initialization

	Audio::GetInstance().InitAudio();
	
	// Init game clocks
	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();

	// Main loop
	while (window.isOpen())
	{
		// Reduce framerate to not spam CPU and GPU
		sf::sleep(sf::milliseconds(16));

		// Calculate time delta
		float currentTime = gameClock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		Game::GetInstance();
		if(!Game::GetInstance().GetCurrentGameState())
		{
			Game::GetInstance().Init();
		}
		Game::GetInstance().GetCurrentGameState()->UpdateGame(window, deltaTime);
		// Read events
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
			Game::GetInstance().GetInputHandler()->UpdateHandler(event, window);
		}
	}
	return 0;
}
