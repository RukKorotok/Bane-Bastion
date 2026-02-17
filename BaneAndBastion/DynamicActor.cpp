#include "pch.h"

#include "DynamicActor.h"
#include <string>

namespace BaneAndBastion
{
	//DynamicActor
	//-----------------------------------------------------------------------------------------------------------
	DynamicActor::DynamicActor(FalkonEngine::Vector2Df position, std::string name, std::string texture)
	{
		auto activeScene = FalkonEngine::Scene::GetActive();
		if (auto* gameScene = dynamic_cast<GameScene*>(activeScene))
		{
			p_gridManager = gameScene->GetGridManager();
			activeScene->GetWorld()->Subscribe(this);
		}
		else
		{
			FE_CORE_ERROR("DynamicActor: Active scene is null or not a GameScene! Actor '" + name + "' will not have a GridManager.");
			p_gridManager = nullptr;
		}

		if (activeScene && activeScene->GetWorld())
		{
			p_gameObject = activeScene->GetWorld()->CreateGameObject<FalkonEngine::Entity>(name);
		}
		else
		{
			FE_CORE_ERROR("DynamicActor: Failed to access World for creating actor '" + name + "'");
			return;
		}

		auto playerRenderer = p_gameObject->AddComponent<FalkonEngine::SpriteRendererComponent>();
		auto sharedTexture = FalkonEngine::ResourceSystem::Instance()->GetTextureShared(texture);

		if (sharedTexture)
		{
			playerRenderer->SetTexture(*sharedTexture);
		}
		else
		{
			FE_CORE_ERROR("DynamicActor: Texture '" + texture + "' not found for actor '" + name + "'");
		}

		playerRenderer->SetPixelSize(GameSettings::PixelsPerUnit * 0.5f, GameSettings::PixelsPerUnit * 0.5f);

		auto transform = p_gameObject->GetComponent<FalkonEngine::TransformComponent>();
		if (transform)
		{
			transform->SetWorldPosition(position);
		}

		FE_APP_TRACE("DynamicActor: '" + name + "' initialized at position (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")");
	}
	//-----------------------------------------------------------------------------------------------------------
	void DynamicActor::Destroy()
	{
		auto activeScene = FalkonEngine::Scene::GetActive();
		if (auto* gameScene = dynamic_cast<GameScene*>(activeScene))
		{
			gameScene->GetWorld()->DestroyGameObject(p_gameObject);
		}

		if (m_isDestroyed) 
		{
			return;
		}
		m_isDestroyed = true;

		FE_APP_INFO("CharacterManager: Entity '" + p_gameObject->GetName() + "' is destroyed.");
	}
	//-----------------------------------------------------------------------------------------------------------
	FalkonEngine::GameObject* DynamicActor::GetGameObject() const
	{
		return p_gameObject;
	}
}
