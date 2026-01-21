#pragma once

#include <string>
#include <cassert>
#include "SFML/Graphics.hpp"

namespace Bane_Bastion
{	

	// Resources path
	const std::string RESOURCES_PATH = "Resources/";

	// Titles
	const std::string SCORE_TITLE = "Score: ";
	const std::string INPUT_TITLE = "Inter name: ";

	// Game config
	const int MAX_NAME_LENGTH = 10;
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const float INITIAL_BALL_SPEED = 400.0f; 
	const float SPETIAL_APPLE_TIME = 20.0f;
	const float BALL_SIZE = 35.0f;
	const float BASE_SEGMENT_SIZE = 35.0f;
	const float BASE_REBOUND_MAX_ANGLE = 80.0f;
	const float CELL_SIZE = 20.0f;
	const int TEXT_SIZE = 24;
	const int MAX_OPPONENTS_SCORE = 50;
	const int BASE_START_SIZE = 3;
	const int PENALTY_STEPS = 20;
	const float START_DELAY = 3.0f;

	// Inputs config
	const sf::Keyboard::Key UP_DIRECTION = sf::Keyboard::Key::W;
	const sf::Keyboard::Key DOWN_DIRECTION = sf::Keyboard::Key::S;
	const sf::Keyboard::Key LEFT_DIRECTION = sf::Keyboard::Key::A;
	const sf::Keyboard::Key RIGHT_DIRECTION = sf::Keyboard::Key::D;
	const sf::Keyboard::Key PAUSE = sf::Keyboard::Key::P;
	const sf::Keyboard::Key ENTER = sf::Keyboard::Key::Enter;
	const sf::Keyboard::Key GO_BACK = sf::Keyboard::Key::B;
}
