#pragma once

#include "AIComponent.h"
#include "Character.h"
#include "AIMoveComponent.h"

namespace BaneAndBastion
{
	class NPC : public Character
	{
	public:
		NPC(FalkonEngine::Vector2Df position, std::string name, std::string texture);
		void HitAction(FalkonEngine::GameObject& gameObject) override;
		void OnNotify(const FalkonEngine::GameEvent& event) override;

	};
}