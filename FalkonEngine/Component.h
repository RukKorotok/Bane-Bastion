#pragma once

#include "SubScriptionSystem.h"

namespace FalkonEngine
{
	class GameObject;

	class Component : public Observable
	{
	public:
		Component(GameObject* gameObject);
		virtual ~Component();

		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;

		GameObject* GetGameObject();
	protected:
		GameObject* p_gameObject;
	};
}