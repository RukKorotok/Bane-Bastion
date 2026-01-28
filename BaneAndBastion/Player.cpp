#include "Player.h"
#include <ResourceSystem.h>

namespace BaneAndBastion
{
	//Player
	//-----------------------------------------------------------------------------------------------------------
	Player::Player()
	{
		m_gameObject = FalkonEngine::GameWorld::Instance()->CreateGameObject<FalkonEngine::Entity>();
		auto playerRenderer = m_gameObject->AddComponent<FalkonEngine::SpriteRendererComponent>();

		playerRenderer->SetTexture(*FalkonEngine::ResourceSystem::Instance()->GetTextureShared("ball"));
		playerRenderer->SetPixelSize(32, 32);

		auto playerCamera = m_gameObject->AddComponent<FalkonEngine::CameraComponent>();
		playerCamera->SetWindow(&FalkonEngine::RenderSystem::Instance()->GetMainWindow());
		playerCamera->SetBaseResolution(1280, 720);


		auto playerInput = m_gameObject->AddComponent<FalkonEngine::InputComponent>();
		if (playerInput) {
			playerInput->Subscribe(this);
		}

		auto playerMovement = m_gameObject->AddComponent<FalkonEngine::MoveComponent>();
		if (playerMovement) {
			playerMovement->Subscribe(this);
		}

		auto transform = m_gameObject->GetComponent<FalkonEngine::TransformComponent>();
		transform->RotateBy(90.f);
		transform->MoveBy({ 1.0f, 0.0f });

		auto test = FalkonEngine::GameWorld::Instance()->CreateGameObject<FalkonEngine::Entity>();
		auto testTransform = test->GetComponent<FalkonEngine::TransformComponent>();
		testTransform->SetParent(transform);

		transform->RotateBy(-90.0f);
		transform->Print();
		testTransform->Print();

		auto collider = m_gameObject->AddComponent<FalkonEngine::SpriteColliderComponent>();
	}

	FalkonEngine::GameObject* Player::GetGameObject()
	{
		return m_gameObject;
	}

	void Player::OnNotify(const FalkonEngine::GameEvent& event)
	{
		switch (event.type) {
			// Логика Ввода -> в Движение
		case FalkonEngine::GameEventType::InputDirectionChanged:
			if (auto move = m_gameObject->GetComponent<FalkonEngine::MoveComponent>())
			{
				move->SetDirection({ event.direction.x, event.direction.y });
			}
			break;

		case FalkonEngine::GameEventType::PositionChanged:
			auto camera = m_gameObject->GetComponent<FalkonEngine::CameraComponent>();
			if (auto transform = m_gameObject->GetComponent<FalkonEngine::TransformComponent>())
			{
				transform->MoveBy(event.direction.x, event.direction.y);
				transform->Print();
			}
			break;
		}
	}
}