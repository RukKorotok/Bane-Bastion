#include "pch.h"

#include "BaneMoveComponent.h"
#include "GameObject.h"

namespace BaneAndBastion
{
    BaneMoveComponent::BaneMoveComponent(FalkonEngine::GameObject* gameObject)
        : MoveComponent(gameObject), m_isActive(false), m_currentPathIndex(0)
    {
        m_currentVelocity = { 0.0f, 0.0f };
        m_desiredDir = { 0.0f, 0.0f };
        m_acceleration = 50.0f;
        m_friction = 100.0f;
    }

    void BaneMoveComponent::LaunchToTarget(const FalkonEngine::Vector2Df& startPos,
        const FalkonEngine::Vector2Df& targetPos,
        IPathfindingStrategy* strategy)
    {
        if (!strategy)
        {
            return;
        }

        auto newPath = strategy->GetPath(startPos, targetPos);

        if (!newPath.empty())
        {
            // Если мы уже в движении, НЕ переписываем m_currentSimulatedPos.
            // Мы просто подменяем рельсы (путь), по которым едем.
            if (!m_isActive)
            {
                m_currentSimulatedPos = startPos;
            }

            m_calculatedPath = std::move(newPath);

            // Магия: убираем рывок назад, делая текущую позицию ПЕРВОЙ точкой
            m_calculatedPath[0] = m_currentSimulatedPos;

            m_currentPathIndex = 0;
            m_isActive = true;
        }
        else
        {
            Stop();
        }
    }

    void BaneMoveComponent::Stop()
    {
        m_isActive = false;
        m_currentPathIndex = 0;
        m_calculatedPath.clear();
        m_currentVelocity = { 0.0f, 0.0f };
        m_desiredDir = { 0.0f, 0.0f };
    }

    void BaneMoveComponent::CalculateDesiredDir(float dt)
    {
        if (!m_isActive || m_currentPathIndex >= m_calculatedPath.size())
        {
            m_desiredDir = { 0.0f, 0.0f };
            return;
        }

        // Берем текущую целевую точку из маршрута
        const FalkonEngine::Vector2Df& targetStep = m_calculatedPath[m_currentPathIndex];

        // Вектор от симулированной позиции до текущей точки пути
        FalkonEngine::Vector2Df toStep = { targetStep.x - m_currentSimulatedPos.x,
                                          targetStep.y - m_currentSimulatedPos.y };

        float distSq = (toStep.x * toStep.x) + (toStep.y * toStep.y);

        if (distSq > m_arrivalThresholdSq)
        {
            // Направление к следующей точке пути
            float len = std::sqrt(distSq);
            m_desiredDir = { toStep.x / len, toStep.y / len };

            // Обновляем виртуальную позицию на основе скорости физического движка (MoveComponent)
            // Это гарантирует, что "умная" логика и физическая инерция синхронизированы
            m_currentSimulatedPos.x += m_currentVelocity.x * dt;
            m_currentSimulatedPos.y += m_currentVelocity.y * dt;
        }
        else
        {
            // Мы достигли промежуточной точки, переходим к следующей
            m_currentPathIndex++;

            // Если путь закончился
            if (m_currentPathIndex >= m_calculatedPath.size())
            {
                m_isActive = false;
                m_desiredDir = { 0.0f, 0.0f };

                // Стандартное уведомление о достижении финальной цели
                FalkonEngine::GameEvent ev;
                ev.type = FalkonEngine::GameEventType::MovementFinished;
                ev.entityID = GetGameObject()->GetID();
                ev.sender = this;
                Notify(ev);
            }
        }
    }
}