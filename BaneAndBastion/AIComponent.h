#pragma once

#include <memory>
#include "Component.h"
#include "Vector.h"
#include "IPathfindingStrategy.h"
#include "CollisionCategories.h"

namespace BaneAndBastion
{
    class GameObject;

    /**
     * @brief Component that drives NPC behavior using pathfinding strategies.
     * * This component monitors a target (usually the player), calculates paths
     * within a detection range, and notifies other systems to move the entity.
     */
    class AIComponent : public FalkonEngine::Component
    {
    public:
        /**
         * @brief Standard constructor for the AI component.
         */
        AIComponent(FalkonEngine::GameObject* gameObject);

        /**
         * @brief Initializes the AI with specific parameters and a navigation strategy.
         * @param target Pointer to the GameObject to track/follow.
         * @param range Distance within which the AI starts seeking the target.
         * @param strategy The pathfinding algorithm to use (A*, Dijkstra, etc.).
         * @param collision The collision mask of this entity to determine passable terrain.
         */
        void Init(FalkonEngine::GameObject* target, float range,
            std::unique_ptr<IPathfindingStrategy> strategy,
            FalkonEngine::CollisionCategory collision);

        /**
         * @brief Switches the current target to a new GameObject.
         */
        void UpdateTarget(FalkonEngine::GameObject* target);

        /**
         * @brief Per-frame logic update for tracking and path recalculation.
         */
        void Update(float deltaTime) override;

        /**
         * @brief Logic-only component; rendering is handled by other components.
         */
        void Render() override {}

    private:
        FalkonEngine::GameObject* m_targetPlayer;   ///< The current focus of the AI.
        float m_detectionRange;                      ///< Seek radius in world units.
        std::unique_ptr<IPathfindingStrategy> m_strategy; ///< Strategy for path generation.
        FalkonEngine::CollisionCategory m_collision; ///< Navigation constraints.
    };
}