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
		auto rd = m_gameObject->AddComponent<FalkonEngine::RigidbodyComponent>();

		if (collider)
		{
			// Подписываемся на лямбда-функцию, которая выполнится при столкновении
			collider->SubscribeCollision([this](FalkonEngine::Collision collision)
				{
					auto otherGO = collision.GetSecond()->GetGameObject();

					if (otherGO->GetComponent<RigidbodyComponent>() != nullptr)
					{
						auto rb = m_gameObject->GetComponent<RigidbodyComponent>();
						if (rb)
						{
							// Движок сам посчитал, куда лететь, мы только даем силу
							rb->ApplyImpulse(collision.GetKnockbackDirection(), 500.f);
						}
					}
				});
		}
	}
}
