#pragma once

#include "DynamicWithPhysic.h"
#include "StatsComponent.h"

namespace BaneAndBastion
{
	class Character : public DynamicWithPhysic

	{
	public:
		Character(FalkonEngine::Vector2Df position, std::string name, std::string texture);
		void OnNotify(const FalkonEngine::GameEvent& event) override {};
	};
}