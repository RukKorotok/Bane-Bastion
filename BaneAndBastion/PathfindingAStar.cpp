#include "pch.h"

#include "PathfindingAStar.h"
#include "GridManager.h"
#include <queue>
#include <unordered_map>
#include <algorithm>

namespace BaneAndBastion
{
    //PathfindingAStar
    //--------------------------------------------------------------------------------------------------------
    FalkonEngine::Vector2Df PathfindingAStar::GetNextStep(FalkonEngine::Vector2Df start, FalkonEngine::Vector2Df target) 
    {
        auto activeScene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive());
        if (!activeScene) 
        {
            return start;
        }

        auto gm = activeScene->GetGridManager();
        if (!gm) 
        {
            return start;
        }

        FalkonEngine::Vector2Di sGrid(gm->WorldToGrid(start.x), gm->WorldToGrid(start.y));
        FalkonEngine::Vector2Di tGrid(gm->WorldToGrid(target.x), gm->WorldToGrid(target.y));

        if (sGrid == tGrid) 
        {
            return target;
        }
        // If player not in grid
        auto cell = gm->GetCell(tGrid.x, tGrid.y);
        if (!cell || cell->entityID != 0) 
        {
            // Not used yet
            return start;
        }

        auto path = PerformAStar(sGrid, tGrid);

        if (path.size() > 1) 
        {
            return gm->GridToWorld(path[1].x, path[1].y);
        }

        // if not find path, go forward
        return start;
    }
    //--------------------------------------------------------------------------------------------------------
    std::vector<FalkonEngine::Vector2Di> PathfindingAStar::PerformAStar(FalkonEngine::Vector2Di start, FalkonEngine::Vector2Di target) 
    {
        auto gm = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive())->GetGridManager();

        std::unordered_map<FalkonEngine::Vector2Di, Node*> allNodes;

        //queue with priority
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
        Node* startNode = new Node{ start, 0, 0, nullptr };
        int dx = abs(target.x - start.x);
        int dy = abs(target.y - start.y);
        startNode->h = 10 * (dx + dy) + (14 - 2 * 10) * std::min(dx, dy);
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
                Node* temp = current;
                while (temp)
                {
                    path.push_back(temp->pos);
                    temp = temp->parent;
                }
                std::reverse(path.begin(), path.end());
                cleanup();
                return path;
            }

            // Òeighboring cells
            static const FalkonEngine::Vector2Di neighbors[8] =
            {
                {0, 1}, {0, -1}, {1, 0}, {-1, 0},
                {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
            };

            for (int i = 0; i < 8; ++i) 
            {
                FalkonEngine::Vector2Di nextPos = current->pos + neighbors[i];
                auto cell = gm->GetCell(nextPos.x, nextPos.y);

                if (!cell || cell->entityID != 0)
                {
                    continue;
                }

                if (i >= 4) 
                {
                    // We check the "passage" between the cells(so as not to cut the corners of the walls)
                    auto side1 = gm->GetCell(current->pos.x + neighbors[i].x, current->pos.y);
                    auto side2 = gm->GetCell(current->pos.x, current->pos.y + neighbors[i].y);
                    if (!side1 || side1->entityID != 0 || !side2 || side2->entityID != 0) 
                    {
                        continue; // Blocked
                    }
                }

                // 10 for direct path, 14 for diagonal)
                int moveCost = (i >= 4) ? 14 : 10;
                int newG = current->g + moveCost;

                auto it = allNodes.find(nextPos);
                if (it == allNodes.end()) 
                {
                    int dX = abs(target.x - nextPos.x);
                    int dY = abs(target.y - nextPos.y);
                    int h = 10 * (dX + dY) + (14 - 2 * 10) * std::min(dX, dY);

                    Node* newNode = new Node{ nextPos, newG, h, current };
                    allNodes[nextPos] = newNode;
                    openSet.push(newNode);
                }
                else if(newG < it->second->g)
                {
                    it->second->g = newG;
                    it->second->parent = current;
                    openSet.push(it->second);
                }
            }
        }
        cleanup();
        return {}; // Not finded
    }
}