#include "pch.h"
#include "PhysicsSystem.h"

namespace FalkonEngine
{
	PhysicsSystem* PhysicsSystem::Instance()
	{
		static PhysicsSystem physicsSystem;
		return &physicsSystem;
	}

	float PhysicsSystem::GetFixedDeltaTime() const
	{
		return m_fixedDeltaTime;
	}

	void PhysicsSystem::Update()
	{
		for (int i = 0; i < m_colliders.size(); i++)
		{
			auto body = m_colliders[i]->GetGameObject()->GetComponent<RigidbodyComponent>();
			if (body == nullptr || body->GetKinematic())
			{
				continue;
			}

			for (int j = 0; j < m_colliders.size(); j++)
			{
				if (j == i)
				{
					continue;
				}

				sf::FloatRect intersection;
				if (m_colliders[i]->m_bounds.intersects(m_colliders[j]->m_bounds, intersection))
				{
					if (m_colliders[i]->m_isTrigger != m_colliders[j]->m_isTrigger)
					{
						if (m_triggersEnteredPair.find(m_colliders[i]) == m_triggersEnteredPair.end() && m_triggersEnteredPair.find(m_colliders[j]) == m_triggersEnteredPair.end())
						{
							auto trigger = new Trigger(m_colliders[i], m_colliders[j]);
							m_colliders[i]->OnTriggerEnter(*trigger);
							m_colliders[j]->OnTriggerEnter(*trigger);

							m_triggersEnteredPair.emplace(m_colliders[i], m_colliders[j]);
						}
					}
					else if (!m_colliders[i]->m_isTrigger)
					{
						float intersectionWidth = intersection.width;
						float intersectionHeight = intersection.height;
						Vector2Df intersectionPosition = { intersection.left - 0.5f * intersectionWidth, intersection.top - 0.5f * intersectionHeight };

						Vector2Df aPosition = { m_colliders[i]->m_bounds.left,  m_colliders[i]->m_bounds.top };
						auto aTransform = m_colliders[i]->GetGameObject()->GetComponent<TransformComponent>();

						if (intersectionWidth > intersectionHeight)
						{
							if (intersectionPosition.y > aPosition.y)
							{
								aTransform->MoveBy({ 0, -intersectionHeight });
								std::cout << "Top collision" << std::endl;
							}
							else
							{
								aTransform->MoveBy({ 0, intersectionHeight });
								std::cout << "Down collision" << std::endl;
							}
						}
						else
						{
							if (intersectionPosition.x > aPosition.x)
							{
								aTransform->MoveBy({ -intersectionWidth, 0.f });
								std::cout << "Right collision" << std::endl;
							}
							else
							{
								aTransform->MoveBy({ intersectionWidth, 0.f });
								std::cout << "Left collision" << std::endl;
							}
						}

						auto collision = new Collision(m_colliders[i], m_colliders[j], intersection);
						m_colliders[i]->OnCollision(*collision);
						m_colliders[j]->OnCollision(*collision);
					}
				}
			}
		}

		for (auto triggeredPair = m_triggersEnteredPair.cbegin(), nextTriggeredPair = triggeredPair; triggeredPair != m_triggersEnteredPair.cend(); triggeredPair = nextTriggeredPair)
		{
			++nextTriggeredPair;
			if (!triggeredPair->first->m_bounds.intersects(triggeredPair->second->m_bounds))
			{
				auto trigger = new Trigger(triggeredPair->first, triggeredPair->second);
				triggeredPair->first->OnTriggerExit(*trigger);
				triggeredPair->second->OnTriggerExit(*trigger);

				m_triggersEnteredPair.erase(triggeredPair);
			}
		}
	}

	void PhysicsSystem::Subscribe(ColliderComponent* collider)
	{
		std::cout << "Subscribe " << collider << std::endl;
		m_colliders.push_back(collider);
	}
	void PhysicsSystem::Unsubscribe(ColliderComponent* collider)
	{
		std::cout << "Unsubscribe " << collider << std::endl;

		m_colliders.erase(std::remove_if(m_colliders.begin(), m_colliders.end(), [collider](ColliderComponent* obj) { return obj == collider; }), m_colliders.end());
	}
}