#pragma once

#include "DynamicActor.h"

namespace BaneAndBastion
{
	class DynamicWithPhysic : public DynamicActor
	{
	public:
		DynamicWithPhysic(FalkonEngine::Vector2Df position, std::string name, std::string texture);
		void OnNotify(const FalkonEngine::GameEvent& event) override {};

	};
}