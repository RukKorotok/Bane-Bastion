#include "pch.h"

#include "DynamicActor.h"

namespace BaneAndBastion
{
	//BaneAndBastion
	//-----------------------------------------------------------------------------------------------------------
	BaneAndBastion::DynamicActor::DynamicActor(FalkonEngine::Vector2Df position, std::string name, std::string texture)
	{
		m_gridManager = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive())->GetGridManager();

		m_gameObject = FalkonEngine::Scene::GetActive()->GetWorld()->CreateGameObject<FalkonEngine::Entity>(name);
		auto playerRenderer = m_gameObject->AddComponent<FalkonEngine::SpriteRendererComponent>();

		playerRenderer->SetTexture(*FalkonEngine::ResourceSystem::Instance()->GetTextureShared(texture));
		playerRenderer->SetPixelSize(GameSettings::PixelsPerUnit * 0.5f, GameSettings::PixelsPerUnit * 0.5f);

		auto transform = m_gameObject->GetComponent<FalkonEngine::TransformComponent>();
		if(transform)
		{
			transform->SetWorldPosition(position);
		}

	}

	FalkonEngine::GameObject* DynamicActor::GetGameObject() const
	{
		return m_gameObject;
	}
}
