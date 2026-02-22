#include "pch.h"

#include "AStarUtilits.h"
#include "GameScene.h"
#include "GridManager.h"
#include <queue>
#include <unordered_map>
#include <algorithm>

namespace BaneAndBastion
{
    //AStarUtils --- Static helper for pathfinding  ---
    //--------------------------------------------------------------------------------------------------------
    std::vector<FalkonEngine::Vector2Di> AStarUtils::FindPath(FalkonEngine::Vector2Di start,
        FalkonEngine::Vector2Di target,
        FalkonEngine::CollisionCategory seekerCategory)
    {

        // Get the GridManager instance from the active GameScene
        auto scene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive());
        if (!scene) return {};
        auto gm = scene->GetGridManager();

        // Memory tracking: stores all allocated nodes to prevent leaks
        std::unordered_map<FalkonEngine::Vector2Di, Node*> allNodes;

        // Priority queue to store nodes to explore. Lowest f cost (g + h) is always on top.
        auto compare = [](Node* a, Node* b) 
            { 
                return a->f() > b->f();
            };
        std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> openSet(compare);

        // Helper function for automatic memory cleanup upon completion
        auto cleanup = [&allNodes]() 
            {
            for (auto& pair : allNodes) 
            {
                delete pair.second;
            }
            };

        // Create the initial node at the starting position
        Node* startNode = new Node{ start, 0, 0, nullptr };

        // Calculate Heuristic (h) using Octile Distance (8-way movement)
        // 10 = cardinal cost, 14 = diagonal cost (~10 * sqrt(2))
        int dx = abs(target.x - start.x);
        int dy = abs(target.y - start.y);
        startNode->h = 10 * (dx + dy) + (14 - 2 * 10) * std::min(dx, dy);

        openSet.push(startNode);
        allNodes[start] = startNode;

        int iterations = 0;
        const int MAX_ITERATIONS = 500; // Performance safety cap to prevent freeze in large maps

        while (!openSet.empty() && iterations < MAX_ITERATIONS)
        {
            iterations++;
            Node* current = openSet.top();
            openSet.pop();

            // Goal check: if reached, reconstruct the path by traversing parents back to start
            if (current->pos == target)
            {
                std::vector<FalkonEngine::Vector2Di> path;
                Node* temp = current;
                while (temp) 
                {
                    path.push_back(temp->pos);
                    temp = temp->parent;
                }
                std::reverse(path.begin(), path.end()); // Reverse to get Start -> Target order
                cleanup();
                return path;
            }

            // Neighbor offsets: 0-3 are Straight (N, S, E, W), 4-7 are Diagonals
            static const FalkonEngine::Vector2Di neighbors[8] = 
            {
                {0, 1}, {0, -1}, {1, 0}, {-1, 0},   // Orthogonal
                {1, 1}, {1, -1}, {-1, 1}, {-1, -1}  // Diagonal
            };

            for (int i = 0; i < 8; ++i)
            {
                FalkonEngine::Vector2Di nextPos = current->pos + neighbors[i];
                auto cell = gm->GetCell(nextPos.x, nextPos.y);

                // ACCESSIBILITY CHECK
                // Skip if cell is out of bounds or blocked for the seeker's category (using bitmask logic)
                if (!cell || cell->Blocks(seekerCategory)) 
                {
                    continue;
                }

                // DIAGONAL PASSABILITY CHECK (Corner Cutting Prevention)
                 // When moving diagonally, ensure both adjacent orthogonal cells are also passable.
                 // This prevents the agent from glitching through thin wall corners.
                if (i >= 4)
                {
                    auto side1 = gm->GetCell(current->pos.x + neighbors[i].x, current->pos.y);
                    auto side2 = gm->GetCell(current->pos.x, current->pos.y + neighbors[i].y);

                    // Если хоть одна из боковых ячеек непроходима для нас - диагональ закрыта
                    if (!side1 || side1->Blocks(seekerCategory) || !side2 || side2->Blocks(seekerCategory)) 
                    {
                        continue;
                    }
                }

                // Determine step cost: 14 for diagonals, 10 for straight moves
                int moveCost = (i >= 4) ? 14 : 10;
                int newG = current->g + moveCost;

                auto it = allNodes.find(nextPos);
                if (it == allNodes.end())
                {
                    // New discovered location: calculate heuristic and add to Open Set
                    int dX = abs(target.x - nextPos.x);
                    int dY = abs(target.y - nextPos.y);
                    int h = 10 * (dX + dY) + (14 - 2 * 10) * std::min(dX, dY);

                    Node* newNode = new Node{ nextPos, newG, h, current };
                    allNodes[nextPos] = newNode;
                    openSet.push(newNode);
                }
                else if (newG < it->second->g)
                {
                    // Found a shorter path to an already visited location: update costs and parent
                    it->second->g = newG;
                    it->second->parent = current;
                    openSet.push(it->second);
                }
            }
        }
        cleanup();
        return {};
    }
}