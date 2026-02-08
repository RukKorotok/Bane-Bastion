#include "pch.h"

#include "NPC.h"
#include "Player.h"
#include "PathfindingAStar.h"

namespace BaneAndBastion 
{
    //NPC
    //--------------------------------------------------------------------------------------------------------
    NPC::NPC(FalkonEngine::Vector2Df position, std::string name, std::string texture) : DynamicWithPhysic(position, name, texture)
    {
        auto movement = m_gameObject->AddComponent<AIMoveComponent>();
        if (movement)
        {
            movement->Subscribe(this);
            movement->SetSpeed(50.0f);

            if (auto* activeScene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive()))
            {
                movement->Subscribe(activeScene->GetGridManager());
            }
        }

        auto aiComponent = m_gameObject->AddComponent<AIComponent>();
        if (aiComponent)
        {
            auto gameScene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive());
            auto strategyAStar = std::make_unique<PathfindingAStar>();
            auto player = gameScene->GetPlayer()->GetGameObject();
            aiComponent->Init(player, 270.0f, std::move(strategyAStar));
            aiComponent->Subscribe(this);
        }

        auto rb = m_gameObject->GetComponent<FalkonEngine::RigidbodyComponent>();
        auto collider = m_gameObject->GetComponent<FalkonEngine::SpriteColliderComponent>();

        if (rb)
        {
            rb->SetLinearDamping(5.0f);
            rb->SetKinematic(false);
        }
	}
    //--------------------------------------------------------------------------------------------------------
	void NPC::OnNotify(const FalkonEngine::GameEvent& event)
	{
        switch (event.type) 
        {
        case GameEventType::MovementRequested: 
        {

            auto moveComp = m_gameObject->GetComponent<AIMoveComponent>();
            auto transform = m_gameObject->GetComponent<FalkonEngine::TransformComponent>();
            if (moveComp && transform)
            {
                moveComp->MoveTowards(transform->GetWorldPosition(), { event.direction.x, event.direction.y });
            }
            break;
        }

        case GameEventType::PositionChanged: 
        {
            auto transform = m_gameObject->GetComponent<FalkonEngine::TransformComponent>();
            if (transform)
            {
                transform->MoveBy(event.direction.x, event.direction.y);
            }
            break;
        }

    }
	}
}
   