#pragma once

#include "Player.h"
#include "IChainAnchor.h"
#include "BaneMoveComponent.h"
#include "PathfindingAStar.h"
#include "Vector.h"

namespace BaneAndBastion
{
    class Bane : public Player
    {
    public:
        Bane(FalkonEngine::Vector2Df position, std::shared_ptr<IChainAnchor> anchor);
        virtual ~Bane() = default;

        // Главный узел обработки логики
        void OnNotify(const FalkonEngine::GameEvent& event) override;

    private:
        bool isMovementAllowed(const FalkonEngine::Vector2Df& targetPos) const;
        void applyChainConstraint();

        std::weak_ptr <IChainAnchor> m_anchor;
        Vector2Di m_lastTargetGrid;


        // Храним стратегии как поля, чтобы не создавать их каждый раз
        std::unique_ptr<IPathfindingStrategy> m_defaultStrategy;

    };
}