#include "InputHandlers.h"
#include "Menu.h"
#include "Game.h"

namespace Bane_Bastion
{
	//------------------------------------------------------------------------------------------------------------
	void InputHandler::UpdateHandler(const sf::Event& event, sf::RenderWindow& window)
	{
		if (event.type == sf::Event::KeyPressed) {
			if (!m_currentKeyStates[event.key.code]) {
				OnInput(event.key.code);
				m_isNewPressFrame = true; 
			}
			m_currentKeyStates[event.key.code] = true;
		}
		else if (event.type == sf::Event::KeyReleased) {
			m_currentKeyStates[event.key.code] = false;
			m_isNewPressFrame = false;
		}

		if (event.type == sf::Event::TextEntered) {
			if (m_isNewPressFrame) {
				OnInputText(event.text.unicode);
				m_isNewPressFrame = false;
			}
		}

		float screenWidth = static_cast<float>(window.getSize().x);
		float mouseX = (float)sf::Mouse::getPosition(window).x;
		if (mouseX < 0) mouseX = 0;
		if (mouseX > screenWidth) mouseX = screenWidth;
		OnChangedMousePosition(mouseX);
	}
}
