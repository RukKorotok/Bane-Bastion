#include "pch.h"

#include "NPC.h"
#include "Player.h"
#include "PathfindingAStar.h"
#include "DamageComponent.h"

namespace BaneAndBastion
{
    //NPC
    //--------------------------------------------------------------------------------------------------------
    NPC::NPC(FalkonEngine::Vector2Df position, std::string name, std::string texture) : Character(position, name, texture)
    {
        auto activeScene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive());
        if (!activeScene)
        {
            FE_CORE_ERROR("NPC: Failed to create '" + name + "' - No active GameScene found.");
            return;
        }

        auto movement = p_gameObject->AddComponent<AIMoveComponent>();
        if (movement)
        {
            movement->Subscribe(this);
            movement->SetSpeed(50.0f);
            movement->Subscribe(activeScene->GetGridManager());
        }

        auto aiComponent = p_gameObject->AddComponent<AIComponent>();
        auto player = activeScene->GetPlayer()->GetGameObject();
        if (aiComponent && player)
        {
            auto strategyAStar = std::make_unique<PathfindingAStar>();
            aiComponent->Init(player, 270.0f, std::move(strategyAStar));
            aiComponent->Subscribe(this);
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
                {StatType::Defense, 0.05f},
                {StatType::Strength, 10.0f }
                });
        }

        auto attack = p_gameObject->AddComponent<DamageComponent>();
        if (attack && stats)
        {
            float damage = stats->GetStat(StatType::Strength);
            attack->SetBaseDamage(damage);
            attack->SetDamageType(0);
            attack->SetHitRate(1.0f);
        }

        FE_APP_TRACE("NPC: '" + name + "' spawned successfully.");
    }
    //--------------------------------------------------------------------------------------------------------
    void NPC::HitAction(FalkonEngine::GameObject& gameObject)
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
    void NPC::OnNotify(const FalkonEngine::GameEvent& event)
    {
        switch (event.type)
        {
        case GameEventType::MovementRequested:
        {

            auto moveComp = p_gameObject->GetComponent<AIMoveComponent>();
            auto transform = p_gameObject->GetComponent<FalkonEngine::TransformComponent>();
            if (moveComp && transform)
            {
                moveComp->MoveTowards(transform->GetWorldPosition(), { event.direction.x, event.direction.y });
            }
            break;
        }

        case GameEventType::PositionChanged:
        {
            auto transform = p_gameObject->GetComponent<FalkonEngine::TransformComponent>();
            if (transform)
            {
                transform->MoveBy(event.direction.x, event.direction.y);
            }
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
                    Destroy();
                }
                else
                {
                    FE_APP_TRACE(p_gameObject->GetName() + " HP is now: " + std::to_string(currentHP));
                }
            }
        }
        }
        DynamicActor::OnNotify(event);
    }
}
   