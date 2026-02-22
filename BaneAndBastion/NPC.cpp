#include "pch.h"

#include "NPC.h"
#include "Player.h"
#include "PathfindingAStar.h"
#include "DamageComponent.h"

namespace BaneAndBastion
{
    // NPC Implementation
    // --------------------------------------------------------------------------------------------------------
    NPC::NPC(FalkonEngine::Vector2Df position) : Character(position, "NPC", "monster", FalkonEngine::CollisionCategory::Enemy)
    {
        auto activeScene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive());
        if (!activeScene)
        {
            FE_CORE_ERROR("NPC: Failed to create 'NPC' - No active GameScene found.");
            return;
        }

        // 1. Setup Movement: Attach AIMoveComponent and set navigation speed
        auto movement = p_gameObject->AddComponent<AIMoveComponent>();
        if (movement)
        {
            movement->Subscribe(this);
            movement->SetSpeed(50.0f);
            movement->Subscribe(activeScene->GetGridManager()); // Register with Grid for obstacle avoidance
        }

        // 2. Setup AI: Initialize the A* strategy and tracking range
        auto aiComponent = p_gameObject->AddComponent<AIComponent>();
        auto player = activeScene->GetPlayer()->GetGameObject();
        if (aiComponent && player)
        {
            auto strategyAStar = std::make_unique<PathfindingAStar>();
            aiComponent->Init(player, 270.0f, std::move(strategyAStar), GetCollisionCategory());
            aiComponent->Subscribe(this);
        }

        // 3. Setup Physics: Configure the Rigidbody for natural movement feel
        auto rb = p_gameObject->GetComponent<FalkonEngine::RigidbodyComponent>();
        if (rb)
        {
            rb->SetLinearDamping(15.0f); // High damping for snappy stops
            rb->SetKinematic(false);     // Allow force-based interactions
            rb->Subscribe(this);
        }

        // 4. Setup Stats: Initialize Health, Strength, and Defense
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

        // 5. Setup Combat: Sync DamageComponent with initialized Strength stat
        auto attack = p_gameObject->AddComponent<DamageComponent>();
        if (attack && stats)
        {
            float damage = stats->GetStat(StatType::Strength);
            attack->SetBaseDamage(damage);
            attack->SetDamageType(0);
            attack->SetHitRate(1.0f);
        }

        FE_APP_TRACE("NPC: spawned successfully.");
    }

    // --------------------------------------------------------------------------------------------------------
    void NPC::HitAction(FalkonEngine::GameObject& gameObject)
    {
        // Damage Calculation: Check if the hitter is not the NPC itself
        if (gameObject.GetID() != p_gameObject->GetID())
        {
            auto attack = gameObject.GetComponent<DamageComponent>();
            auto stats = p_gameObject->GetComponent<StatsComponent>();

            if (attack && stats)
            {
                // Apply damage modified by the Defense stat percentage
                float damage = attack->HitTrigger() * abs(1 - stats->GetStat(StatType::Defense));
                stats->ChangeStat(StatType::Health, damage * -1.0f);
            }
        }
    }

    // --------------------------------------------------------------------------------------------------------
    void NPC::OnNotify(const FalkonEngine::GameEvent& event)
    {
        // Centralized Event Bus: Handling internal component communication
        switch (event.type)
        {
        case GameEventType::MovementRequested:
        {
            // Triggered when AI pathfinding decides on a new target coordinate
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
            // Update the physical Transform whenever a MoveComponent calculates a new delta
            auto transform = p_gameObject->GetComponent<FalkonEngine::TransformComponent>();
            if (transform)
            {
                transform->MoveBy(event.direction.x, event.direction.y);
            }
            break;
        }

        case FalkonEngine::GameEventType::HitVelocityRequest:
        {
            // Handle knockback or external forces (e.g., being hit by a heavy attack)
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
            // Handle visual rotation cues (e.g., spinning when taking damage)
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
            // Check for death conditions when health values are updated
            StatType changedStat = static_cast<StatType>(event.actionID);
            if (changedStat == StatType::Health)
            {
                float currentHP = event.value;
                if (currentHP <= 0.0f)
                {
                    Destroy(); // Kill the entity if health reaches zero
                }
                else
                {
                    FE_APP_TRACE(p_gameObject->GetName() + " HP is now: " + std::to_string(currentHP));
                }
            }
            break;
        }
        }
        // Forward event to parent for basic dynamic actor logic
        DynamicActor::OnNotify(event);
    }
}