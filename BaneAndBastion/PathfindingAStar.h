#pragma once

#include "IPathfindingStrategy.h"
#include "AStarUtilits.h"
#include "GameScene.h"
#include "GridManager.h"

namespace BaneAndBastion
{
    class PathfindingAStar : public IPathfindingStrategy
    {
    public:
        std::vector<FalkonEngine::Vector2Df> GetPath(FalkonEngine::Vector2Df start, FalkonEngine::Vector2Df target)
        {
            auto scene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive());
            auto gm = scene->GetGridManager();

            auto gridPath = AStarUtils::FindPath(
                { gm->WorldToGrid(start.x), gm->WorldToGrid(start.y) },
                { gm->WorldToGrid(target.x), gm->WorldToGrid(target.y) }
            );

            std::vector<FalkonEngine::Vector2Df> worldPath;
            worldPath.reserve(gridPath.size());

            for (const auto& pos : gridPath) 
            {
                worldPath.push_back(gm->GridToWorld(pos.x, pos.y));
            }

            return worldPath;
        }
    };
}