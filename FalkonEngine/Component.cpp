#include "pch.h"
#include "Component.h"
#include <iostream>

namespace FalkonEngine
{
	//Component
	//-----------------------------------------------------------------------------------------------------------
	Component::Component(GameObject* gameObject) : p_gameObject(gameObject) 
	{
		FE_CORE_ASSERT(p_gameObject != nullptr, "Component created without a valid GameObject pointer!");
	}
	//-----------------------------------------------------------------------------------------------------------
	Component::~Component()
	{
		FE_APP_TRACE("Deleted component: " + std::to_string(reinterpret_cast<uintptr_t>(this)));
	}
	//-----------------------------------------------------------------------------------------------------------
	GameObject* Component::GetGameObject()
	{
		FE_CORE_ASSERT(p_gameObject != nullptr, "Component's owner GameObject is null during GetGameObject() call!");
		return p_gameObject;
	}
}