#include "pch.h"
#include "Bastion.h"
#include "GameObject.h"
#include "PlayerMoveComponent.h"
#include "DamageComponent.h"

namespace BaneAndBastion 
{
	//Bastion
	//--------------------------------------------------------------------------------------------------------
    Bastion::Bastion(FalkonEngine::Vector2Df position) : Player(position, "Bastion", "knight") 
    {
		auto playerCamera = p_gameObject->AddComponent<FalkonEngine::CameraComponent>();
		playerCamera->SetWindow(&FalkonEngine::RenderSystem::Instance()->GetMainWindow());
		playerCamera->SetBaseResolution(1024, 720);

		auto movement = p_gameObject->AddComponent<PlayerMoveComponent>();
		if (movement)
		{
			movement->Subscribe(this);
			movement->SetSpeed(100.0f);

			if (auto* activeScene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive()))
			{
				movement->Subscribe(activeScene->GetGridManager());
			}
		}

		if (auto* activeScene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive()))
		{
			p_gridManager->UpdateVisibleArea(position, 1);
		}

		auto rb = p_gameObject->GetComponent<FalkonEngine::RigidbodyComponent>();

		if (rb)
		{
			rb->SetLinearDamping(15.0f);
			rb->SetKinematic(false);
			rb->Subscribe(this);
		}

		auto stats = p_gameObject->GetComponent<StatsComponent>();

		if (stats)
		{
			stats->InitStats({
				{StatType::Health, 100.0f},
				{StatType::MaxHealth, 100.0f},
				{StatType::Defense, 5.0f}
				});
		}
    }
	//--------------------------------------------------------------------------------------------------------
    FalkonEngine::Vector2Df Bastion::GetAnchorPosition() const 
	{
		auto transform = p_gameObject->GetComponent<FalkonEngine::TransformComponent>();
		if(transform)
		{
			return transform->GetWorldPosition();
		}
    }
	//--------------------------------------------------------------------------------------------------------
    float Bastion::GetMaxChainDistance() const 
	{ 
		return m_maxChainDistance;
	}
	//--------------------------------------------------------------------------------------------------------
	void Bastion::HitAction(FalkonEngine::GameObject& gameObject)
	{
		if (gameObject.GetID() != p_gameObject->GetID())
		{
			auto attack = gameObject.GetComponent<DamageComponent>();
			auto stats = p_gameObject->GetComponent<StatsComponent>();

			if (attack && stats)
			{
				float damage = attack->HitTrigger() * abs(1 - stats->GetStat(StatType::Defense));
				stats->ChangeStat(StatType::Health, damage * -1.0f);
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------
    void Bastion::OnNotify(const FalkonEngine::GameEvent& event) 
	{
		switch (event.type)
		{
		case FalkonEngine::GameEventType::InputDirectionChanged:
		{
			if (auto move = p_gameObject->GetComponent<PlayerMoveComponent>())
			{
				move->SetTargetDirection({ event.direction.x, event.direction.y });
			}
			break;
		}
		case FalkonEngine::GameEventType::PositionChanged:
		{
			FalkonEngine::GameEvent ev;
			ev.type = FalkonEngine::GameEventType::MovementRequested;
			ev.entityID = GetGameObject()->GetID();
			ev.sender = this;
			Notify(ev);
			break;
		}
		case FalkonEngine::GameEventType::HitVelocityRequest:
		{
			Vector2Df hitVelocity = event.direction * event.value;
			auto transform = p_gameObject->GetComponent<TransformComponent>();

			if (transform)
			{
				transform->MoveBy(hitVelocity);
			}
			break;
		}
		case FalkonEngine::GameEventType::HitRotationRequest:
		{
			float hitRotation = event.angle * event.value;
			auto transform = p_gameObject->GetComponent<TransformComponent>();

			if (transform)
			{
				transform->RotateBy(hitRotation);
			}
			break;
		}
		case FalkonEngine::GameEventType::StatChanged:
		{
			StatType changedStat = static_cast<StatType>(event.actionID);

			if (changedStat == StatType::Health)
			{
				float currentHP = event.value;

				if (currentHP <= 0.0f)
				{
					//this->Destroy(); //For HW tests
				}
				else
				{
					FE_APP_TRACE(p_gameObject->GetName() + " HP is now: " + std::to_string(currentHP));
				}
			}
		}
        }
		Player::OnNotify(event);
    }
}