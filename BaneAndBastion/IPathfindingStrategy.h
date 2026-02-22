#pragma once

#include "Vector.h"
#include <vector>
#include "CollisionCategories.h"

namespace BaneAndBastion
{
    /**
     * @brief Interface for pathfinding algorithms.
     * * Defines a common contract for all pathfinding implementations,
     * allowing NPCs to use different navigation logic interchangeably.
     */
    class IPathfindingStrategy
    {
    public:
        /**
         * @brief Virtual destructor for proper cleanup of derived classes.
         */
        virtual ~IPathfindingStrategy() = default;

        /**
         * @brief Pure virtual method to calculate a path between two points.
         * * @param start Initial position in world space.
         * @param target Destination point in world space.
         * @param collision The collision category of the agent (to determine passable areas).
         * @return A vector of world coordinates representing the path from start to target.
         */
        virtual std::vector<FalkonEngine::Vector2Df> GetPath(FalkonEngine::Vector2Df start
            , FalkonEngine::Vector2Df target, FalkonEngine::CollisionCategory collision) = 0;
    };
}