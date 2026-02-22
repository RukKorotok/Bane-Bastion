#include "pch.h"
#include "MazeGenerator.h"
#include "Wall.h"
//#include "DungeonFloor.h"
#include "GameSettings.h"
#include <stack>

namespace BaneAndBastion
{
    MazeGenerator::MazeGenerator()
    {
        // Maze walls are usually solid and consistent
        RegisterAsset<Wall>(1.0f);
    }

    std::vector<EnvironmentObject*> MazeGenerator::Generate(int cx, int cy
        , FalkonEngine::Vector2Di safeGridPos, int safeRadiusCells)
    {
        std::vector<EnvironmentObject*> objects;
        int size = GameSettings::ChunkSize;

        // Logical grid: 0 = Wall, 1 = Path
        std::vector<std::vector<int>> grid(size, std::vector<int>(size, 0));

        // Seed based on chunk coordinates for deterministic maze layouts
        std::mt19937 gen(std::hash<int>{}(cx) ^ (std::hash<int>{}(cy) << 1));

        // Pre-process safe zone to ensure players never spawn inside a wall
        for (int x = 0; x < size; ++x)
        {
            for (int y = 0; y < size; ++y)
            {
                int gx = cx * size + x;
                int gy = cy * size + y;

                if (std::abs(gx - safeGridPos.x) <= safeRadiusCells &&
                    std::abs(gy - safeGridPos.y) <= safeRadiusCells) {
                    grid[x][y] = 1; // Mark as path
                }
            }
        }

        // DFS Maze Generation Algorithm
        std::stack<FalkonEngine::Vector2Di> stack;
        FalkonEngine::Vector2Di startPos(1, 1);

        if (grid[1][1] == 0) grid[1][1] = 1;
        stack.push(startPos);

        // Movement directions with a step of 2 to leave walls between paths
        FalkonEngine::Vector2Di dirs[] = { {0, 2}, {0, -2}, {2, 0}, {-2, 0} };

        while (!stack.empty())
        {
            auto curr = stack.top();
            std::vector<FalkonEngine::Vector2Di> neighbors;

            for (auto& d : dirs)
            {
                int nx = curr.x + d.x;
                int ny = curr.y + d.y;

                // Check bounds and ensure we only visit uncarved cells (0)
                if (nx > 0 && nx < size - 1 && ny > 0 && ny < size - 1 && grid[nx][ny] == 0)
                {
                    neighbors.push_back({ nx, ny });
                }
            }

            if (!neighbors.empty())
            {
                // Choose a random neighbor to move to
                auto next = neighbors[std::uniform_int_distribution<int>(0, (int)neighbors.size() - 1)(gen)];

                grid[next.x][next.y] = 1; // Mark the next cell as path

                // Remove the wall between the current cell and the next cell
                grid[curr.x + (next.x - curr.x) / 2][curr.y + (next.y - curr.y) / 2] = 1;

                stack.push(next);
            }
            else
            {
                stack.pop(); // Backtrack
            }
        }

        // Object instantiation based on the generated logical grid
        for (int x = 0; x < size; ++x)
        {
            for (int y = 0; y < size; ++y)
            {
                float wx = (cx * size + x) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
                float wy = (cy * size + y) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;

                if (grid[x][y] == 0)
                {
                    objects.push_back(new Wall(FalkonEngine::Vector2Df(wx, wy)));
                }
                else
                {
                    // Optionally spawn floor tiles here
                    // objects.push_back(new DungeonFloor(wx, wy));
                }
            }
        }
        return objects;
    }
}