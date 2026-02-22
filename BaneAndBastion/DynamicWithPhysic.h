#pragma once

#include "DynamicActor.h"
#include "GameScene.h"

namespace BaneAndBastion
{
	class DynamicWithPhysic : public DynamicActor
	{
	public:
		DynamicWithPhysic(FalkonEngine::Vector2Df position, const std::string& name, const std::string& texture, FalkonEngine::CollisionCategory collision);
		virtual void HitAction(FalkonEngine::GameObject& gameObject) {};
		void OnNotify(const FalkonEngine::GameEvent& event) override {};

	};
}