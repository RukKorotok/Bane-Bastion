#pragma once

#include "MoveComponent.h"
#include "IPathfindingStrategy.h"
#include <vector>
#include "CollisionCategories.h"

namespace BaneAndBastion
{
    /**
     * @brief Specialized movement component for the "Bane" entity.
     * * Handles autonomous navigation towards a specific world-space target
     * by overriding core steering behavior.
     */
    class BaneMoveComponent : public MoveComponent
    {
    public:
        /**
         * @brief Standard constructor for the Bane movement component.
         */
        BaneMoveComponent(FalkonEngine::GameObject* gameObject);

        virtual ~BaneMoveComponent() = default;

        /**
         * @brief Initializes the movement task towards a target coordinate.
         * @param startPos Initial world position of the entity.
         * @param targetPos World-space destination to reach.
         */
        void LaunchToTarget(const FalkonEngine::Vector2Df& startPos,
            const FalkonEngine::Vector2Df& targetPos);

        /**
         * @brief Immediately halts the entity and deactivates the current movement task.
         * Overrides the base MoveComponent behavior.
         */
        void Stop() override;

        /**
         * @brief Calculates the steering direction based on the current target vector.
         * Logic is executed if m_isActive is set to true.
         * @param dt Delta time for the current frame.
         */
        virtual void CalculateDesiredDir(float dt) override;

    private:
        /**
         * @brief Custom friction logic to slow down the entity over time.
         */
        void ApplyFriction(float dt);

        FalkonEngine::Vector2Df m_calculatedTarget; ///< Internal world destination.
        FalkonEngine::Vector2Df m_startPose;        ///< Cached starting position for current segment.
        bool m_isActive = false;                    ///< Flag to enable/disable automated steering.

        /**
         * @brief Tolerance threshold (squared distance) for arriving at the target point.
         * Prevents "jittering" behavior when the entity is very close to the goal.
         */
        const float m_arrivalThresholdSq = 20.0f;

    };
}