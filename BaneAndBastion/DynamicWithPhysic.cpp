#include "pch.h"

#include "DynamicWithPhysic.h"
#include "SpriteColliderComponent.h"
#include "RigidbodyComponent.h"

namespace BaneAndBastion
{
	//BaneAndBastion
	//-----------------------------------------------------------------------------------------------------------
	BaneAndBastion::DynamicWithPhysic::DynamicWithPhysic(FalkonEngine::Vector2Df position, std::string name, std::string texture) : DynamicActor(position, name, texture)
	{
		auto collider = m_gameObject->AddComponent<FalkonEngine::SpriteColliderComponent>();
		auto rb = m_gameObject->AddComponent<FalkonEngine::RigidbodyComponent>();

		if (collider && rb)
		{
			collider->SubscribeCollision([this](const FalkonEngine::Collision& collision)
				{

					auto otherCollider = (collision.GetFirst() == m_gameObject->GetComponent<FalkonEngine::SpriteColliderComponent>())
						? collision.GetSecond()
						: collision.GetFirst();

					if (!otherCollider) 
					{
						return;
					}

					auto otherGO = otherCollider->GetGameObject();
					if (!otherGO) 
					{
						return;
					}

					if (otherGO->GetComponent<FalkonEngine::RigidbodyComponent>() != nullptr)
					{
						auto myRB = m_gameObject->GetComponent<FalkonEngine::RigidbodyComponent>();
						if (myRB)
						{
							FalkonEngine::Vector2Df dir = collision.GetKnockbackDirection();

							if (collision.GetSecond()->GetGameObject() == m_gameObject)
							{
								dir = { -dir.x, -dir.y };
							}

							myRB->ApplyImpulse(dir, 500.f);

							FE_APP_TRACE("DynamicWithPhysic: '" + m_gameObject->GetName() +
								"' bounced off '" + otherGO->GetName() + "'");
						}
					}
				});
		}
		else
		{
			FE_CORE_ERROR("DynamicWithPhysic: Failed to add physics components to '" + name + "'");
		}
	}
}
