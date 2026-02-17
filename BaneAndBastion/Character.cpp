#include "pch.h"

#include "Character.h"
#include "SpriteColliderComponent.h"

namespace BaneAndBastion
{
	//BaneAndBastion
	//-----------------------------------------------------------------------------------------------------------
	Character::Character(FalkonEngine::Vector2Df position, std::string name, std::string texture) : DynamicWithPhysic(position, name, texture)
	{
		auto stats = p_gameObject->AddComponent<FalkonEngine::StatsComponent>();
		stats->Subscribe(this);
	}
}
