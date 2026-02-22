#include "Player.h"
#include <ResourceSystem.h>
#include "GameSettings.h"

namespace BaneAndBastion
{
	// Player
	// -----------------------------------------------------------------------------------------------------------
	Player::Player(FalkonEngine::Vector2Df position, const std::string& name, const std::string& texture, FalkonEngine::CollisionCategory collision)
		: Character(position, name, texture, collision)
	{
		// 1. Initialize user input handling
		auto playerInput = p_gameObject->AddComponent<FalkonEngine::InputComponent>();
		if (playerInput)
		{
			// Subscribe the Player to input events to translate key presses into movement/actions
			playerInput->Subscribe(this);
		}
	}

	// -----------------------------------------------------------------------------------------------------------
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

			// 2. Grid-based Collision Resolution
			if (p_gridManager)
			{
				// Step-by-step axis verification to allow "sliding" along walls

				// Check X-axis movement
				sf::FloatRect currentBounds = collider->GetBounds();
				sf::FloatRect boundsX = currentBounds;
				boundsX.left += event.direction.x;

				if (!p_gridManager->CheckGridCollision(boundsX, GetCollisionCategory()))
				{
					transform->MoveBy(event.direction.x, 0.f);
				}

				// Check Y-axis movement independently
				sf::FloatRect boundsY = collider->GetBounds();
				boundsY.top += event.direction.y;

				if (!p_gridManager->CheckGridCollision(boundsY, GetCollisionCategory()))
				{
					transform->MoveBy(0.f, event.direction.y);
				}
			}
			else
			{
				// Default movement if no grid manager is present (no collision)
				transform->MoveBy(event.direction.x, event.direction.y);
			}
			break;
		}
		}
	}
}