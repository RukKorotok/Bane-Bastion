#include "pch.h"
#include "InputComponent.h"
#include "GameObject.h"

namespace FalkonEngine
{
	//InputComponent
	//-----------------------------------------------------------------------------------------------------------
	InputComponent::InputComponent(GameObject* gameObject) : Component(gameObject) 
	{
		FE_APP_TRACE("InputComponent initialized for: " + p_gameObject->GetName());
	}
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

			std::string debugMsg = "Input changed on '" + p_gameObject->GetName() +
				"': H=" + std::to_string(m_horizontalAxis) +
				", V=" + std::to_string(m_verticalAxis);

			FE_APP_TRACE(debugMsg);

			try
			{
				FalkonEngine::GameEvent event;
				event.type = FalkonEngine::GameEventType::InputDirectionChanged;
				event.sender = this;
				event.direction = { m_horizontalAxis, m_verticalAxis };

				Notify(event);
			}
			catch (const std::exception& e)
			{
				std::string errorMsg = "Exception during Input Notification on '" +
					p_gameObject->GetName() + "': " + e.what();

				FE_CORE_ERROR(errorMsg);
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void InputComponent::Render(){}
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