#include "pch.h"
#include "Component.h"
#include <iostream>

namespace FalkonEngine
{
	//Component
	//-----------------------------------------------------------------------------------------------------------
	Component::Component(GameObject* gameObject) : p_gameObject(gameObject) {}
	//-----------------------------------------------------------------------------------------------------------
	Component::~Component()
	{
		std::cout << "Deleted component: " << this << std::endl;
	}
	//-----------------------------------------------------------------------------------------------------------
	GameObject* Component::GetGameObject()
	{
		return p_gameObject;
	}
}