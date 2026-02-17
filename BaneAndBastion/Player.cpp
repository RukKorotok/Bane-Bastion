#include "Player.h"
#include <ResourceSystem.h>
#include "GameSettings.h"

namespace BaneAndBastion
{
	//Player
	//-----------------------------------------------------------------------------------------------------------
	Player::Player(FalkonEngine::Vector2Df position, std::string name, std::string texture) : Character(position, name, texture)
	{
		auto playerInput = p_gameObject->AddComponent<FalkonEngine::InputComponent>();
		if (playerInput)
		{
			playerInput->Subscribe(this);
		}
	}

	void Player::OnNotify(const FalkonEngine::GameEvent& event)
	{
		switch (event.type)
		{
		case FalkonEngine::GameEventType::PositionChanged:
		{
			auto collider = p_gameObject->GetComponent<FalkonEngine::SpriteColliderComponent>();
			auto transform = p_gameObject->GetComponent<FalkonEngine::TransformComponent>();

			if (!transform || !collider) 
			{
				return;
			}

			if (p_gridManager)
			{
				sf::FloatRect currentBounds = collider->GetBounds();
				sf::FloatRect boundsX = currentBounds;
				boundsX.left += event.direction.x;
				if (!p_gridManager->CheckGridCollision(boundsX, p_gameObject->GetID()))
				{
					transform->MoveBy(event.direction.x, 0.f);
				}

				sf::FloatRect boundsY = collider->GetBounds();
				boundsY.top += event.direction.y;
				if (!p_gridManager->CheckGridCollision(boundsY, p_gameObject->GetID()))
				{
					transform->MoveBy(0.f, event.direction.y);
				}
			}
			else
			{
				transform->MoveBy(event.direction.x, event.direction.y);
			}
			break;
		}
		}
	}
}