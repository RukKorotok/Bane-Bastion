#pragma once

#include "MoveComponent.h"

namespace BaneAndBastion
{
    /**
     * @brief Movement component specialized for AI-controlled entities.
     * * Inherits physical properties (velocity, acceleration) from MoveComponent
     * but replaces user input with automated target-seeking logic.
     */
    class AIMoveComponent : public MoveComponent
    {
    public:
        AIMoveComponent(FalkonEngine::GameObject* gameObject);

        /**
         * @brief Updates the component's state and integrates velocity.
         * Calls the base MoveComponent::Update to handle physical movement.
         */
        void Update(float deltaTime) override { MoveComponent::Update(deltaTime); }

        /**
         * @brief Logic-only component; no visual representation required.
         */
        void Render() override {}

        /**
         * @brief Immediately halts the entity and deactivates any active path following.
         */
        void Stop() override;

        /**
         * @brief Initiates movement from a starting point towards a world-space target.
         * @param start Initial position for the movement segment.
         * @param target Destination point to reach.
         */
        void MoveTowards(FalkonEngine::Vector2Df start, FalkonEngine::Vector2Df target);

    protected:
        /**
         * @brief Calculates the steering direction towards m_targetPos.
         * Overrides the base class method to provide autonomous direction logic.
         */
        void CalculateDesiredDir(float dt) override;

    private:
        FalkonEngine::Vector2Df m_startPos{ 0, 0 };  ///< Current segment starting position.
        FalkonEngine::Vector2Df m_targetPos{ 0, 0 }; ///< Target destination in world coordinates.
        bool m_active = false;                       ///< Whether the component is actively steering towards a target.

    };
}