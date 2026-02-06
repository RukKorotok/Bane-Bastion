#pragma once

#include "AIComponent.h"
#include "DynamicWithPhysic.h"
#include "AIMoveComponent.h"

namespace BaneAndBastion
{
	class NPC : public DynamicWithPhysic
	{
	public:
		NPC(FalkonEngine::Vector2Df position, std::string name, std::string texture);
		void OnNotify(const FalkonEngine::GameEvent& event) override;

	};
}