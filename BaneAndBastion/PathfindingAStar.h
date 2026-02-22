#pragma once

#include "IPathfindingStrategy.h"
#include "AStarUtilits.h"
#include "GameScene.h"
#include "GridManager.h"

namespace BaneAndBastion
{
    /**
     * @brief Implementation of the A* pathfinding strategy.
     * * This class acts as a wrapper that converts world coordinates (Vector2Df)
     * into grid coordinates, calculates the path using AStarUtils,
     * and converts the result back to world units.
     */
    class PathfindingAStar : public IPathfindingStrategy
    {
    public:
        /**
         * @brief Calculates a path in world space coordinates.
         * * @param start The starting position in world units.
         * @param target The target destination in world units.
         * @param collision The collision category of the agent (used for traversability checks).
         * @return A vector of world space positions representing the path.
         */
        std::vector<FalkonEngine::Vector2Df> GetPath(FalkonEngine::Vector2Df start,
            FalkonEngine::Vector2Df target,
            FalkonEngine::CollisionCategory collision) override
        {
            auto scene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive());
            if (!scene) return {}; // Safety check for active scene

            auto gm = scene->GetGridManager();

            // 1. Convert world coordinates to grid indices for the algorithm
            auto gridPath = AStarUtils::FindPath(
                { gm->WorldToGrid(start.x), gm->WorldToGrid(start.y) },
                { gm->WorldToGrid(target.x), gm->WorldToGrid(target.y) },
                collision);

            // 2. Prepare the result container
            std::vector<FalkonEngine::Vector2Df> worldPath;
            worldPath.reserve(gridPath.size());

            // 3. Convert calculated grid path back to world coordinates
            for (const auto& pos : gridPath)
            {
                worldPath.push_back(gm->GridToWorld(pos.x, pos.y));
            }

            return worldPath;
        }
    };
}