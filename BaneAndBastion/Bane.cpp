#include "pch.h"
#include "Bane.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "DamageComponent.h"

namespace BaneAndBastion
{
    Bane::Bane(FalkonEngine::Vector2Df pos, std::shared_ptr<IChainAnchor> anchor) : Player(pos, "Bane", "knight"), m_anchor(anchor)
    {
        m_defaultStrategy = std::make_unique<PathfindingAStar>();
        // Инициализируем компонент перемещения
        auto movement = p_gameObject->AddComponent<BaneMoveComponent>();
        movement->Subscribe(this);
        movement->SetSpeed(1000.0f);

        if (auto* activeScene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive()))
        {
            movement->Subscribe(activeScene->GetGridManager());
        }

        auto rb = p_gameObject->GetComponent<FalkonEngine::RigidbodyComponent>();

        if (rb)
        {
            rb->SetLinearDamping(5.0f);
            rb->SetKinematic(false);
        }

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
        // 1. Отрабатываем базовую логику (физика/сетка из Player)
       // Player::OnNotify(event);

        switch (event.type)
        {
        case FalkonEngine::GameEventType::MouseMoved:
        {
            FalkonEngine::Vector2Df currentPos = p_gameObject->GetComponent<FalkonEngine::TransformComponent>()->GetWorldPosition();
            auto movement = p_gameObject->GetComponent<BaneMoveComponent>();

            uint32_t myID = GetGameObject()->GetID();
            FalkonEngine::Vector2Df correctedTarget = p_gridManager->GetNearestPassablePoint(event.direction, myID);

            float ppu = static_cast<float>(GameSettings::PixelsPerUnit);
            int targetGridX = p_gridManager->WorldToGrid(correctedTarget.x);
            int targetGridY = p_gridManager->WorldToGrid(correctedTarget.y);
            if (!isMovementAllowed(event.direction))
            {
                break;
            }
            // Пересчитываем путь ТОЛЬКО если целевая КЛЕТКА изменилась
            if (targetGridX != m_lastTargetGrid.x || targetGridY != m_lastTargetGrid.y)
            {
                m_lastTargetGrid.x = targetGridX;
                m_lastTargetGrid.y = targetGridY;
                movement->LaunchToTarget(currentPos, event.direction, m_defaultStrategy.get());
            }
            break;
        }

        case FalkonEngine::GameEventType::PositionChanged:
        {
            // Каждый раз, когда тра
                Player::OnNotify(event);
                //applyChainConstraint();
            break;
        }
        case FalkonEngine::GameEventType::MovementRequested:
        {
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
        if (!anchor)
        {
            return true;
        }

        FalkonEngine::Vector2Df anchorPos = anchor->GetAnchorPosition();
        float maxDist = anchor->GetMaxChainDistance();

        float targetDist = (targetPos - anchorPos).GetLength();

        // Разрешаем только если цель ВНУТРИ круга
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

        // Если Бастион уходит и "тащит" нас за собой
        if (currentDist > maxDist)
        {
            FalkonEngine::Vector2Df direction = offset;
            direction.Normalize(); // Твой метод

            // Ставим Bane на привязь
            FalkonEngine::Vector2Df constrainedPos = anchorPos + (direction * maxDist);
            transform->SetWorldPosition(constrainedPos);
        }
    }
}