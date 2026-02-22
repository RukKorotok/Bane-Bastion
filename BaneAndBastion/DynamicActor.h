#pragma once

#include "SpriteRendererComponent.h"
#include "MoveComponent.h"
#include "ResourceSystem.h"
#include "Entity.h"
#include "Vector.h"
#include "Actor.h"

namespace BaneAndBastion
{
	class GridManager;

	class DynamicActor : public FalkonEngine::Actor
	{
	public:
		DynamicActor(FalkonEngine::Vector2Df position, const std::string& name, const std::string& texture, FalkonEngine::CollisionCategory collision);
		void OnNotify(const FalkonEngine::GameEvent& event) override {}

	protected:
		GridManager* p_gridManager = nullptr;

	};
}