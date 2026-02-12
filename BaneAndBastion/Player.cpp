#include "Player.h"
#include <ResourceSystem.h>
#include "GameSettings.h"

namespace BaneAndBastion
{
	//Player
	//-----------------------------------------------------------------------------------------------------------
	Player::Player(FalkonEngine::Vector2Df position, std::string name, std::string texture) : DynamicWithPhysic(position, name, texture)
	{
		auto playerCamera = m_gameObject->AddComponent<FalkonEngine::CameraComponent>();
		playerCamera->SetWindow(&FalkonEngine::RenderSystem::Instance()->GetMainWindow());
		playerCamera->SetBaseResolution(1024, 720);

		auto playerInput = m_gameObject->AddComponent<FalkonEngine::InputComponent>();
		if (playerInput) 
		{
			playerInput->Subscribe(this);
		}

		auto movement = m_gameObject->AddComponent<PlayerMoveComponent>();
		if (movement)
		{
			movement->Subscribe(this);
			movement->SetSpeed(100.0f);

			if (auto* activeScene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive()))
			{
				movement->Subscribe(activeScene->GetGridManager());
			}
		}

		if (auto* activeScene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive()))
		{
			m_gridManager->UpdateVisibleArea(position, 1);
		}

		auto transform = m_gameObject->GetComponent<FalkonEngine::TransformComponent>();
		auto test = FalkonEngine::Scene::GetActive()->GetWorld()->CreateGameObject<FalkonEngine::Entity>();
		auto testTransform = test->GetComponent<FalkonEngine::TransformComponent>();
		testTransform->SetParent(transform);

		auto rb = m_gameObject->GetComponent<FalkonEngine::RigidbodyComponent>();
		auto collider = m_gameObject->GetComponent<FalkonEngine::SpriteColliderComponent>();

		if (rb)
		{
			rb->SetLinearDamping(5.0f);
			rb->SetKinematic(false);
		}

	}

	void Player::OnNotify(const FalkonEngine::GameEvent& event)
	{
		switch (event.type)
		{
		case FalkonEngine::GameEventType::InputDirectionChanged:
		{
			if (auto move = m_gameObject->GetComponent<PlayerMoveComponent>())
			{
				move->SetTargetDirection({ event.direction.x, event.direction.y });
			}
			break;
		}
		case FalkonEngine::GameEventType::PositionChanged:
		{
			auto collider = m_gameObject->GetComponent<FalkonEngine::SpriteColliderComponent>();
			auto transform = m_gameObject->GetComponent<FalkonEngine::TransformComponent>();
			auto rb = m_gameObject->GetComponent<FalkonEngine::RigidbodyComponent>();

			if (!transform || !collider) 
			{
				return;
			}

			if (m_gridManager)
			{
				sf::FloatRect currentBounds = collider->GetBounds();
				sf::FloatRect boundsX = currentBounds;
				boundsX.left += event.direction.x;
				if (!m_gridManager->CheckGridCollision(boundsX, m_gameObject->GetID()))
				{
					transform->MoveBy(event.direction.x, 0.f);
				}
				sf::FloatRect boundsY = collider->GetBounds();
				boundsY.top += event.direction.y;
				if (!m_gridManager->CheckGridCollision(boundsY, m_gameObject->GetID()))
				{
					transform->MoveBy(0.f, event.direction.y);
				}

				m_gridManager->RenderDebug(FalkonEngine::RenderSystem::Instance()->GetMainWindow());
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