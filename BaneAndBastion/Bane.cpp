#include "pch.h"
#include "Bane.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "DamageComponent.h"

namespace BaneAndBastion
{
    Bane::Bane(FalkonEngine::Vector2Df pos, std::shared_ptr<IChainAnchor> anchor)
        : Player(pos, "Bane", "knight", FalkonEngine::CollisionCategory::Player), m_anchor(anchor)
    {
        // 1. Setup Movement: Attach specialized BaneMoveComponent for mouse-seeking logic
        auto movement = p_gameObject->AddComponent<BaneMoveComponent>();
        movement->Subscribe(this);
        movement->SetSpeed(400.0f);

        // Register with GridManager for tile-based collision resolution
        if (auto* activeScene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive()))
        {
            movement->Subscribe(activeScene->GetGridManager());
        }

        // 2. Setup Physics: Configure Rigidbody with lower damping for "slippery" movement
        auto rb = p_gameObject->GetComponent<FalkonEngine::RigidbodyComponent>();
        if (rb)
        {
            rb->SetLinearDamping(5.0f);
            rb->SetKinematic(false);
        }

        // 3. Setup Stats: Bane has higher strength but the same base health as the anchor
        auto stats = p_gameObject->GetComponent<StatsComponent>();
        if (stats)
        {
            stats->InitStats({
                {StatType::Health, 100.0f},
                {StatType::MaxHealth, 100.0f},
                {StatType::Defense, 5.0f},
                {StatType::Strength, 30.0f }
                });
        }

        // 4. Setup Combat: Higher hit rate (0.5s) compared to standard NPCs
        auto attack = p_gameObject->AddComponent<DamageComponent>();
        if (attack && stats)
        {
            float damage = stats->GetStat(StatType::Strength);
            attack->SetBaseDamage(damage);
            attack->SetDamageType(0);
            attack->SetHitRate(0.5f);
        }
    }

    void Bane::OnNotify(const FalkonEngine::GameEvent& event)
    {
        switch (event.type)
        {
        case FalkonEngine::GameEventType::MouseMoved:
        {
            auto movement = p_gameObject->GetComponent<BaneMoveComponent>();
            if (!movement) break;

            // Validate if the intended mouse target is within the allowed chain radius
            if (!isMovementAllowed(event.direction)) break;

            FalkonEngine::Vector2Df mousePos = event.direction;
            FalkonEngine::Vector2Df currentPos = p_gameObject->GetComponent<FalkonEngine::TransformComponent>()->GetWorldPosition();

            // Precision check: Stop movement if within a 20px radius of the target
            float distSq = std::pow(mousePos.x - currentPos.x, 2) + std::pow(mousePos.y - currentPos.y, 2);
            if (distSq < 40.0f)
            {
                movement->Stop();
                break;
            }

            // Optimization: Only update pathing if the mouse has moved more than 5px
            float deltaTargetSq = std::pow(mousePos.x - m_lastRawTarget.x, 2) + std::pow(mousePos.y - m_lastRawTarget.y, 2);
            if (deltaTargetSq > 5.0f)
            {
                m_lastRawTarget = mousePos;
                movement->LaunchToTarget(currentPos, mousePos);
            }
            break;
        }

        case FalkonEngine::GameEventType::PositionChanged:
        {
            // Forward movement delta to base Player class for grid-based collision checks
            Player::OnNotify(event);
            break;
        }
        case FalkonEngine::GameEventType::MovementRequested:
        {
            // Enforce tether constraints before the final transformation
            applyChainConstraint();
            break;
        }
        case FalkonEngine::GameEventType::MovementFinished:
        {
            FE_APP_TRACE("Bane reached the target!");
            break;
        }
        }
    }

    bool Bane::isMovementAllowed(const FalkonEngine::Vector2Df& targetPos) const
    {
        auto anchor = m_anchor.lock();
        if (!anchor) return true;

        FalkonEngine::Vector2Df anchorPos = anchor->GetAnchorPosition();
        float maxDist = anchor->GetMaxChainDistance();

        // Calculate distance between intended target and the anchor point
        float targetDist = (targetPos - anchorPos).GetLength();

        // Allow movement only if target is within the allowed spherical/circular constraint
        return (targetDist <= maxDist);
    }

    void Bane::applyChainConstraint()
    {
        auto anchor = m_anchor.lock();
        if (!anchor) return;

        FalkonEngine::Vector2Df anchorPos = anchor->GetAnchorPosition();
        float maxDist = anchor->GetMaxChainDistance();

        auto transform = p_gameObject->GetComponent<FalkonEngine::TransformComponent>();
        FalkonEngine::Vector2Df currentPos = transform->GetWorldPosition();

        FalkonEngine::Vector2Df offset = currentPos - anchorPos;
        float currentDist = offset.GetLength();

        // "Leash" Logic: If current position exceeds max distance, pull Bane back to the constraint edge
        if (currentDist > maxDist)
        {
            FalkonEngine::Vector2Df direction = offset;
            direction.Normalize();

            // Set world position exactly at the constraint boundary
            FalkonEngine::Vector2Df constrainedPos = anchorPos + (direction * maxDist);
            transform->SetWorldPosition(constrainedPos);
        }
    }
}