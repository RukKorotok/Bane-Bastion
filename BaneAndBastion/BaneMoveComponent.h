#pragma once

#include "MoveComponent.h"
#include "IPathfindingStrategy.h"
#include <vector>

namespace BaneAndBastion
{
    class BaneMoveComponent : public MoveComponent
    {
    public:
        BaneMoveComponent(FalkonEngine::GameObject* gameObject);
        virtual ~BaneMoveComponent() = default;

        // »нициализаци€ движени€: передаем текущую позицию, цель и стратегию поиска
        void LaunchToTarget(const FalkonEngine::Vector2Df& startPos,
            const FalkonEngine::Vector2Df& targetPos,
            IPathfindingStrategy* strategy);

        // ѕринудительна€ остановка
        void Stop() override;

        // –асчет направлени€ на основе текущей точки из массива m_calculatedPath
        virtual void CalculateDesiredDir(float dt) override;

    private:
        std::vector<FalkonEngine::Vector2Df> m_calculatedPath;
        FalkonEngine::Vector2Df m_currentSimulatedPos;
        size_t m_currentPathIndex = 0;
        bool m_isActive = false;

        // ѕорог дистанции до точки (квадрат рассто€ни€)
        const float m_arrivalThresholdSq = 20.0f;

    };
}