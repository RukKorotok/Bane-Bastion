#include "pch.h"

#include "Character.h"
#include "SpriteColliderComponent.h"

namespace BaneAndBastion
{
	//BaneAndBastion
	//-----------------------------------------------------------------------------------------------------------
	Character::Character(FalkonEngine::Vector2Df position, std::string name, std::string texture, FalkonEngine::CollisionCategory collision)
		: DynamicWithPhysic(position, name, texture, collision)
	{
		auto stats = p_gameObject->AddComponent<FalkonEngine::StatsComponent>();
		stats->Subscribe(this);
	}
}
