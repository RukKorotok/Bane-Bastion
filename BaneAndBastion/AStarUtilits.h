#pragma once

#include "Vector.h"
#include "CollisionCategories.h"
#include <vector>

namespace BaneAndBastion
{
    /**
     * @brief Represents a single point in the pathfinding grid.
     */
    struct Node
    {
        FalkonEngine::Vector2Di pos; ///< Grid coordinates of the node.
        int g;                       ///< Cost from the start node to this node.
        int h;                       ///< Heuristic: estimated cost from this node to the target.
        Node* parent;                ///< Link to the previous node in the path.

        /**
         * @brief Calculates the total estimated cost.
         * @return Sum of g and h costs.
         */
        int f() const { return g + h; }
    };

    /**
     * @brief Static utility class for A* pathfinding operations.
     */
    class AStarUtils
    {
    public:
        /**
         * @brief Finds a path between two points using the A* algorithm.
         * * @param start Initial grid position.
         * @param target Destination grid position.
         * @param seekerCategory Collision category of the agent to check traversability.
         * @return A vector of grid coordinates representing the path. Returns empty if no path found.
         */
        static std::vector<FalkonEngine::Vector2Di> FindPath(FalkonEngine::Vector2Di start,
            FalkonEngine::Vector2Di target,
            FalkonEngine::CollisionCategory seekerCategory);
    };
}