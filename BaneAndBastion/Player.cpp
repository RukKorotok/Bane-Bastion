#include "Player.h"
#include <ResourceSystem.h>

namespace BaneAndBastion
{
	Player::Player()
	{
		gameObject = FalkonEngine::GameWorld::Instance()->CreateGameObject();
		auto playerRenderer = gameObject->AddComponent<FalkonEngine::SpriteRendererComponent>();

		playerRenderer->SetTexture(*FalkonEngine::ResourceSystem::Instance()->GetTextureShared("ball"));
		playerRenderer->SetPixelSize(32, 32);

		auto playerCamera = gameObject->AddComponent<FalkonEngine::CameraComponent>();
		playerCamera->SetWindow(&FalkonEngine::RenderSystem::Instance()->GetMainWindow());
		playerCamera->SetBaseResolution(1280, 720);

		auto playerInput = gameObject->AddComponent<FalkonEngine::InputComponent>();
	}

	FalkonEngine::GameObject* Player::GetGameObject()
	{
		return gameObject;
	}
}