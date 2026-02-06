#include "pch.h"
#include "InputComponent.h"

namespace FalkonEngine
{
	//InputComponent
	//-----------------------------------------------------------------------------------------------------------
	InputComponent::InputComponent(GameObject* gameObject) : Component(gameObject) {}
	//-----------------------------------------------------------------------------------------------------------
	void InputComponent::Update(float deltaTime)
	{
		float currentVertical = 0.f;
		float currentHorizontal = 0.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			currentVertical += 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			currentVertical -= 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			currentHorizontal += 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			currentHorizontal -= 1.0f;
		}

		if (currentVertical != m_verticalAxis || currentHorizontal != m_horizontalAxis)
		{
			m_verticalAxis = currentVertical;
			m_horizontalAxis = currentHorizontal;

			FalkonEngine::GameEvent event;
			event.type = FalkonEngine::GameEventType::InputDirectionChanged;
			event.sender = this;
			event.direction = { m_horizontalAxis, m_verticalAxis }; // x = Horizontal, y = Vertical

			Notify(event);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void InputComponent::Render()
	{

	}
	//-----------------------------------------------------------------------------------------------------------
	float InputComponent::GetHorizontalAxis() const
	{
		return m_horizontalAxis;
	}
	//-----------------------------------------------------------------------------------------------------------
	float InputComponent::GetVerticalAxis() const
	{
		return m_verticalAxis;
	}
}