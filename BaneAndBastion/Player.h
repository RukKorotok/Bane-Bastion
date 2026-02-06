#pragma once

#include "CameraComponent.h"
#include "InputComponent.h"
#include "DynamicWithPhysic.h"
#include "PlayerMoveComponent.h"

namespace BaneAndBastion
{
	class Player : public DynamicWithPhysic
	{
	public:
		Player(FalkonEngine::Vector2Df position, std::string name, std::string texture);
		void OnNotify(const FalkonEngine::GameEvent& event) override;

	};
}