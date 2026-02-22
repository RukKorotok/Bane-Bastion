#include "pch.h"

#include "DynamicWithPhysic.h"
#include "SpriteColliderComponent.h"
#include "RigidbodyComponent.h"

namespace BaneAndBastion
{
	//BaneAndBastion
	//-----------------------------------------------------------------------------------------------------------
	BaneAndBastion::DynamicWithPhysic::DynamicWithPhysic(FalkonEngine::Vector2Df position, const std::string& name, const std::string& texture, FalkonEngine::CollisionCategory collision) : DynamicActor(position, name, texture, collision)
	{
		auto collider = p_gameObject->AddComponent<FalkonEngine::SpriteColliderComponent>();
		auto rb = p_gameObject->AddComponent<FalkonEngine::RigidbodyComponent>();

		if (collider && rb)
		{
			collider->SubscribeCollision([this](const FalkonEngine::Collision& collision)
				{

					auto otherCollider = (collision.GetFirst() == p_gameObject->GetComponent<FalkonEngine::SpriteColliderComponent>())
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
						auto myRB = p_gameObject->GetComponent<FalkonEngine::RigidbodyComponent>();
						if (myRB)
						{
							FalkonEngine::Vector2Df dir = collision.GetKnockbackDirection();

							if (collision.GetSecond()->GetGameObject() == p_gameObject)
							{
								dir = { -dir.x, -dir.y };
							}

							myRB->ApplyImpulse(dir, 500.f);
							HitAction(*otherGO);
							FE_APP_TRACE("DynamicWithPhysic: '" + p_gameObject->GetName() +
								"' bounced off '" + otherGO->GetName() + "'");
						}
					}
				});
			collider->SetCategory(collision);
		}
		else
		{
			FE_CORE_ERROR("DynamicWithPhysic: Failed to add physics components to '" + name + "'");
		}
	}
}
