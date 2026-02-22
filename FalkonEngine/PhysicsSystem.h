#pragma once

#include <map>
#include <iostream>
#include "ColliderComponent.h"
#include "RigidbodyComponent.h"
#include "Vector.h"
#include "CollisionRegistry.h"

namespace FalkonEngine
{
	class PhysicsSystem
	{
	public:
		static PhysicsSystem* Instance();

		void Update();

		float GetFixedDeltaTime() const;
		void Subscribe(ColliderComponent* collider);
		void Unsubscribe(ColliderComponent* collider);
	private:
		PhysicsSystem() {}
		~PhysicsSystem() {}

		PhysicsSystem(PhysicsSystem const&) = delete;
		PhysicsSystem& operator= (PhysicsSystem const&) = delete;

		std::vector<ColliderComponent*> m_colliders;
		std::map<ColliderComponent*, ColliderComponent*> m_triggersEnteredPair;

		float m_fixedDeltaTime = 0.02f;
	};
}