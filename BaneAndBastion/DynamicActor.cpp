#include "pch.h"

#include "DynamicActor.h"
#include "GridManager.h"
#include <string>
#include "GameScene.h"

namespace BaneAndBastion
{
	//DynamicActor
	//-----------------------------------------------------------------------------------------------------------
	DynamicActor::DynamicActor(FalkonEngine::Vector2Df position, const std::string& name, const std::string& texture, FalkonEngine::CollisionCategory collision)
		: FalkonEngine::Actor(position, name, texture, collision)
	{
		auto activeScene = FalkonEngine::Scene::GetActive();
		if (auto* gameScene = dynamic_cast<GameScene*>(activeScene))
		{
			if (p_gridManager = gameScene->GetGridManager())
			{
				activeScene->GetWorld()->Subscribe(this);
			}
		}
		else
		{
			FE_CORE_ERROR("DynamicActor: Active scene is null or not a GameScene! Actor '" + name + "' will not have a GridManager.");
			p_gridManager = nullptr;
		}
		auto playerRenderer = p_gameObject->GetComponent<SpriteRendererComponent>();
		playerRenderer->SetPixelSize(GameSettings::PixelsPerUnit * 0.5f, GameSettings::PixelsPerUnit * 0.5f);

		FE_APP_TRACE("DynamicActor: '" + name + "' initialized at position (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")");
	}
}
