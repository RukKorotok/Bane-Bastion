#include "pch.h"

#include "Bastion.h"
#include "GameObject.h"
#include "PlayerMoveComponent.h"
#include "DamageComponent.h"
#include "RenderSystem.h"

namespace BaneAndBastion
{
    // Bastion Implementation
    // --------------------------------------------------------------------------------------------------------
    Bastion::Bastion(FalkonEngine::Vector2Df position) : Player(position, "Bastion", "knight", FalkonEngine::CollisionCategory::Player)
    {
        // 1. Setup Camera: Bind the main window to the player's view
        auto playerCamera = p_gameObject->AddComponent<FalkonEngine::CameraComponent>();
        playerCamera->SetWindow(&FalkonEngine::RenderSystem::Instance()->GetMainWindow());
        playerCamera->SetBaseResolution(1024, 720);

        // 2. Setup Movement: Add the player-controlled movement component
        auto movement = p_gameObject->AddComponent<PlayerMoveComponent>();
        if (movement)
        {
            movement->Subscribe(this);
            movement->SetSpeed(100.0f);

            // Register with GridManager for tile-based collision detection
            if (auto* activeScene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive()))
            {
                movement->Subscribe(activeScene->GetGridManager());
            }
        }

        // 3. Setup Physics: Configure Rigidbody for kinetic interactions
        auto rb = p_gameObject->GetComponent<FalkonEngine::RigidbodyComponent>();
        if (rb)
        {
            rb->SetLinearDamping(15.0f);
            rb->SetKinematic(false);
            rb->Subscribe(this);
        }

        // 4. Setup Stats: Initialize base health and defense values
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

    // --------------------------------------------------------------------------------------------------------
    FalkonEngine::Vector2Df Bastion::GetAnchorPosition() const
    {
        // Retrieves world coordinates for secondary systems (like the chain/tether system)
        auto transform = p_gameObject->GetComponent<FalkonEngine::TransformComponent>();
        if (transform)
        {
            return transform->GetWorldPosition();
        }
        return { 0.0f, 0.0f };
    }

    // --------------------------------------------------------------------------------------------------------
    float Bastion::GetMaxChainDistance() const
    {
        return m_maxChainDistance;
    }

    // --------------------------------------------------------------------------------------------------------
    void Bastion::HitAction(FalkonEngine::GameObject& gameObject)
    {
        // Combat logic: Calculate incoming damage modified by defense stats
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

    // --------------------------------------------------------------------------------------------------------
    void Bastion::OnNotify(const FalkonEngine::GameEvent& event)
    {
        // Central event dispatcher for player-specific behaviors
        switch (event.type)
        {
        case FalkonEngine::GameEventType::InputDirectionChanged:
        {
            // Bridge between the Input system and the Movement component
            if (auto move = p_gameObject->GetComponent<PlayerMoveComponent>())
            {
                move->SetTargetDirection({ event.direction.x, event.direction.y });
            }
            break;
        }
        case FalkonEngine::GameEventType::PositionChanged:
        {
            // Relay movement updates to other systems (e.g., the chain system or triggers)
            FalkonEngine::GameEvent ev;
            ev.type = FalkonEngine::GameEventType::MovementRequested;
            ev.entityID = GetGameObject()->GetID();
            ev.sender = this;
            Notify(ev);
            break;
        }
        case FalkonEngine::GameEventType::HitVelocityRequest:
        {
            // Apply external translation forces (knockback)
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
            // Apply external rotation forces (impact visual effects)
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
            // Monitor vitals and handle character death logic
            StatType changedStat = static_cast<StatType>(event.actionID);
            if (changedStat == StatType::Health)
            {
                float currentHP = event.value;
                if (currentHP <= 0.0f)
                {
                    // Handle player death (e.g., Game Over screen or Respawn)
                }
                else
                {
                    FE_APP_TRACE(p_gameObject->GetName() + " HP is now: " + std::to_string(currentHP));
                }
            }
            break;
        }
        }
        // Pass event to the base Player class for grid-collision handling
        Player::OnNotify(event);
    }
}