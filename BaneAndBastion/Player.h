#pragma once

#include "CameraComponent.h"
#include "InputComponent.h"
#include "Character.h"
#include "PlayerMoveComponent.h"

namespace BaneAndBastion
{
	class Player : public Character
	{
	public:
		Player(FalkonEngine::Vector2Df position, const std::string& name, const std::string& texture, FalkonEngine::CollisionCategory collision);
		void OnNotify(const FalkonEngine::GameEvent& event) override;

	};
}