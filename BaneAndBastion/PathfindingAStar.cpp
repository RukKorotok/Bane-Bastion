#include "pch.h"

#include "PathfindingAStar.h"
#include "GridManager.h"
#include <queue>
#include <unordered_map>
#include <algorithm>

namespace BaneAndBastion
{

    FalkonEngine::Vector2Df PathfindingAStar::GetNextStep(FalkonEngine::Vector2Df start, FalkonEngine::Vector2Df target) {
        auto gm = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive())->GetGridManager();
        FalkonEngine::Vector2Di sGrid(gm->WorldToGrid(start.x), gm->WorldToGrid(start.y));
        FalkonEngine::Vector2Di tGrid(gm->WorldToGrid(target.x), gm->WorldToGrid(target.y));

        // If player not in grid
        auto cell = gm->GetCell(tGrid.x, tGrid.y);
        if (!cell || cell->entityID != 0) {
            // Not used yet
            return start;
        }

        if (sGrid == tGrid)
        {
            return target;
        }

        auto path = PerformAStar(sGrid, tGrid);

        if (path.size() > 1) 
        {
            return gm->GridToWorld(path[1].x, path[1].y);
        }

        // if not find path, go forward
        return target;
    }

    std::vector<FalkonEngine::Vector2Di> PathfindingAStar::PerformAStar(FalkonEngine::Vector2Di start, FalkonEngine::Vector2Di target) 
    {
        auto gm = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive())->GetGridManager();

        std::unordered_map<FalkonEngine::Vector2Di, Node*> allNodes;

        // 2. queue with priority
        auto compare = [](Node* a, Node* b) 
        { 
            return a->f() > b->f(); 
        };
        std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> openSet(compare);

        // clear memory
        auto cleanup = [&allNodes]() 
        {
            for (auto& pair : allNodes) delete pair.second;
        };

        // Create start point
        Node* startNode = new Node{ start, 0, abs(target.x - start.x) + abs(target.y - start.y) * 10, nullptr };
        openSet.push(startNode);
        allNodes[start] = startNode;

        int iterations = 0;
        const int MAX_ITERATIONS = 500;

        while (!openSet.empty() && iterations < MAX_ITERATIONS) 
        {
            iterations++;
            Node* current = openSet.top();
            openSet.pop();

            // Target reached
            if (current->pos == target) 
            {
                std::vector<FalkonEngine::Vector2Di> path;
                while (current) 
                {
                    path.push_back(current->pos);
                    current = current->parent;
                }
                std::reverse(path.begin(), path.end());
                cleanup();
                return path;
            }

            // Òeighboring cells
            FalkonEngine::Vector2Di neighbors[8] = 
            {
                {0, 1}, {0, -1}, {1, 0}, {-1, 0},
                {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
            };

            for (auto& off : neighbors) 
            {
                FalkonEngine::Vector2Di nextPos = current->pos + off;
                auto cell = gm->GetCell(nextPos.x, nextPos.y);

                if (!cell || cell->entityID != 0)
                {
                    continue;
                }

                bool isDiagonal = (off.x != 0 && off.y != 0);
                if (isDiagonal) 
                {
                    // We check the "passage" between the cells(so as not to cut the corners of the walls)
                    auto side1 = gm->GetCell(current->pos.x + off.x, current->pos.y);
                    auto side2 = gm->GetCell(current->pos.x, current->pos.y + off.y);
                    if (!side1 || side1->entityID != 0 || !side2 || side2->entityID != 0) 
                    {
                        continue; // Blocked
                    }
                }

                // 10 for direct path, 14 for diagonal)
                int moveCost = isDiagonal ? 14 : 10;
                int newG = current->g + moveCost;

                auto it = allNodes.find(nextPos);
                if (it == allNodes.end() || newG < it->second->g) 
                {
                    int h = (abs(target.x - nextPos.x) + abs(target.y - nextPos.y)) * 10;

                    Node* n = new Node{ nextPos, newG, h, current };
                    allNodes[nextPos] = n;
                    openSet.push(n);
                }
            }
        }
        cleanup();
        return {}; // Not finded
    }
}