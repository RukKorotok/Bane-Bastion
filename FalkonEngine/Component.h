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

		// -- (Core functions) --
		/** Called on tick metod, deltaTime its frame duration */
		virtual void Update(float deltaTime) = 0;
		/** Called on tick metod */
		virtual void Render() = 0;

		/** Get parent object */
		GameObject* GetGameObject();

	protected:
		GameObject* p_gameObject;
	};
}