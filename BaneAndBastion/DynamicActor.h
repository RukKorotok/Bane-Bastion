#pragma once

#include "GameScene.h"
#include "SpriteRendererComponent.h"
#include "MoveComponent.h"
#include "ResourceSystem.h"
#include "Entity.h"
#include "Vector.h"

namespace BaneAndBastion
{
	class DynamicActor : public FalkonEngine::Observer
	{
	public:
		DynamicActor(FalkonEngine::Vector2Df position, std::string name, std::string texture);
		FalkonEngine::GameObject* GetGameObject() const;
		void OnNotify(const FalkonEngine::GameEvent& event) override {};

	protected:
		FalkonEngine::GameObject* m_gameObject;
		GridManager* m_gridManager;

	};
}