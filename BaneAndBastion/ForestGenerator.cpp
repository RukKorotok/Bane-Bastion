#include "pch.h"
#include "ForestGenerator.h"
//#include "Tree.h"
#include "Wall.h"
//#include "Grass.h"
#include "GameSettings.h"

namespace BaneAndBastion
{
    ForestGenerator::ForestGenerator()
    {
        // Register environment assets with spawn weights
        // RegisterAsset<Tree>(0.15f); 
        RegisterAsset<Wall>(0.02f);
    }

    std::vector<EnvironmentObject*> ForestGenerator::Generate(int cx, int cy
        , FalkonEngine::Vector2Di safeGridPos, int safeRadiusCells)
    {
        std::vector<EnvironmentObject*> objects;

        // Deterministic seed based on chunk coordinates
        std::mt19937 gen(std::hash<int>{}(cx) ^ (std::hash<int>{}(cy) << 1));

        for (int lx = 0; lx < GameSettings::ChunkSize; ++lx)
        {
            for (int ly = 0; ly < GameSettings::ChunkSize; ++ly)
            {
                // Calculate global grid coordinates
                int gx = cx * GameSettings::ChunkSize + lx;
                int gy = cy * GameSettings::ChunkSize + ly;

                // Calculate world position (centered in cell)
                float wx = gx * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
                float wy = gy * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;

                // Skip generation if within the safe zone radius
                if (std::abs(gx - safeGridPos.x) <= safeRadiusCells &&
                    std::abs(gy - safeGridPos.y) <= safeRadiusCells)
                {
                    // Safe zone is kept clear of obstacles
                    continue;
                }

                // Randomly pick an asset type based on registration weights
                std::type_index selected = GetRandomType(gen);

                if (selected != std::type_index(typeid(void)))
                {
                    // Create object via factory function
                    objects.push_back(m_factories[selected](wx, wy));
                }
                else
                {
                    // Default floor/empty space handling
                }
            }
        }
        return objects;
    }
}