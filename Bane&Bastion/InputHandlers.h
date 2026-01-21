#pragma once
#include "SFML/Graphics.hpp"
#include "Observer.h"
#include <memory>

namespace Bane_Bastion
{
	class InputHandler : public InputObservable
	{
	public:

		InputHandler() {}
		~InputHandler() {}

		void UpdateHandler(const sf::Event& event, sf::RenderWindow& window);

	private:

		InputHandler(InputHandler const&) = delete;
		InputHandler& operator = (InputHandler const&) = delete;
		std::map<sf::Keyboard::Key, bool> m_currentKeyStates;
		std::map<sf::Keyboard::Key, bool> m_previewKeyStates;
		bool m_isNewPressFrame = false;
	};
}
