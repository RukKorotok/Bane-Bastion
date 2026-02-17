#pragma once

#include "DynamicActor.h"

namespace BaneAndBastion
{
	class DynamicWithPhysic : public DynamicActor
	{
	public:
		DynamicWithPhysic(FalkonEngine::Vector2Df position, std::string name, std::string texture);
		virtual void HitAction(FalkonEngine::GameObject& gameObject) {};
		void OnNotify(const FalkonEngine::GameEvent& event) override {};

	};
}