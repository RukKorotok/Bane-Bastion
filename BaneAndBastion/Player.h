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
		Player(FalkonEngine::Vector2Df position, std::string name, std::string texture);
		void OnNotify(const FalkonEngine::GameEvent& event) override;

	};
}