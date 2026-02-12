#include "pch.h"
#include "PhysicsSystem.h"

namespace FalkonEngine
{
	//Physic system
	//-----------------------------------------------------------------------------------------------------------
	PhysicsSystem* PhysicsSystem::Instance()
	{
		static PhysicsSystem physicsSystem;
		return &physicsSystem;
	}
	//-----------------------------------------------------------------------------------------------------------
	float PhysicsSystem::GetFixedDeltaTime() const
	{
		return m_fixedDeltaTime;
	}
	//-----------------------------------------------------------------------------------------------------------
	void PhysicsSystem::Update()
	{
		for (size_t i = 0; i < m_colliders.size(); i++)
		{
			if (!m_colliders[i]) 
			{
				continue;
			}

			auto body = m_colliders[i]->GetGameObject()->GetComponent<RigidbodyComponent>();

			if (body == nullptr || body->GetKinematic()) 
			{
				continue;
			}

			for (size_t j = 0; j < m_colliders.size(); j++)
			{
				if (i == j || !m_colliders[j]) 
				{
					continue;
				}

				sf::FloatRect intersection;
				if (m_colliders[i]->m_bounds.intersects(m_colliders[j]->m_bounds, intersection))
				{
					if (m_colliders[i]->m_isTrigger || m_colliders[j]->m_isTrigger)
					{
						if (m_triggersEnteredPair.find(m_colliders[i]) == m_triggersEnteredPair.end())
						{
							Trigger triggerInfo(m_colliders[i], m_colliders[j]);
							m_colliders[i]->OnTriggerEnter(triggerInfo);
							m_colliders[j]->OnTriggerEnter(triggerInfo);

							m_triggersEnteredPair.emplace(m_colliders[i], m_colliders[j]);
							FE_APP_TRACE("Trigger Enter: " + m_colliders[i]->GetGameObject()->GetName() + " <-> " + m_colliders[j]->GetGameObject()->GetName());
						}
					}
					else
					{
						float intersectionWidth = intersection.width;
						float intersectionHeight = intersection.height;

						Vector2Df aCenter(m_colliders[i]->m_bounds.left + m_colliders[i]->m_bounds.width / 2.f,
							m_colliders[i]->m_bounds.top + m_colliders[i]->m_bounds.height / 2.f);
						Vector2Df bCenter(m_colliders[j]->m_bounds.left + m_colliders[j]->m_bounds.width / 2.f,
							m_colliders[j]->m_bounds.top + m_colliders[j]->m_bounds.height / 2.f);

						auto aTransform = m_colliders[i]->GetGameObject()->GetComponent<TransformComponent>();

						if (intersectionWidth > intersectionHeight)
						{
							if (aCenter.y < bCenter.y) {
								aTransform->MoveBy({ 0.f, -intersectionHeight });
								FE_APP_TRACE("Collision: " + m_colliders[i]->GetGameObject()->GetName() + " TOP");
							}
							else {
								aTransform->MoveBy({ 0.f, intersectionHeight });
								FE_APP_TRACE("Collision: " + m_colliders[i]->GetGameObject()->GetName() + " DOWN");
							}
						}
						else
						{
							if (aCenter.x < bCenter.x) {
								aTransform->MoveBy({ -intersectionWidth, 0.f });
								FE_APP_TRACE("Collision: " + m_colliders[i]->GetGameObject()->GetName() + " RIGHT");
							}
							else {
								aTransform->MoveBy({ intersectionWidth, 0.f });
								FE_APP_TRACE("Collision: " + m_colliders[i]->GetGameObject()->GetName() + " LEFT");
							}
						}

						Collision collisionInfo(m_colliders[i], m_colliders[j], intersection);
						m_colliders[i]->OnCollision(collisionInfo);
						m_colliders[j]->OnCollision(collisionInfo);
					}
				}
			}
		}

		for (auto it = m_triggersEnteredPair.begin(); it != m_triggersEnteredPair.end(); )
		{
			if (!it->first->m_bounds.intersects(it->second->m_bounds))
			{
				Trigger exitInfo(it->first, it->second);
				it->first->OnTriggerExit(exitInfo);
				it->second->OnTriggerExit(exitInfo);

				FE_APP_TRACE("Trigger Exit: " + it->first->GetGameObject()->GetName());
				it = m_triggersEnteredPair.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void PhysicsSystem::Subscribe(ColliderComponent* collider)
	{
		if (!collider) 
		{
			return;
		}
		m_colliders.push_back(collider);
		FE_APP_TRACE("PhysicsSystem: Collider subscribed (" + collider->GetGameObject()->GetName() + ")");
	}
	//-----------------------------------------------------------------------------------------------------------
	void PhysicsSystem::Unsubscribe(ColliderComponent* collider)
	{
		std::cout << "Unsubscribe " << collider << std::endl;

		m_colliders.erase(std::remove_if(m_colliders.begin(), m_colliders.end(), [collider](ColliderComponent* obj) { return obj == collider; }), m_colliders.end());



		auto it = std::remove(m_colliders.begin(), m_colliders.end(), collider);
		if (it != m_colliders.end())
		{
			m_colliders.erase(it, m_colliders.end());
			FE_APP_TRACE("PhysicsSystem: Collider unsubscribed");
		}

		for (auto tIt = m_triggersEnteredPair.begin(); tIt != m_triggersEnteredPair.end(); )
		{
			if (tIt->first == collider || tIt->second == collider)
			{
				tIt = m_triggersEnteredPair.erase(tIt);
			}
			else
			{
				++tIt;
			}
		}
	}
}