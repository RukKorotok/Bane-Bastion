#include "pch.h"
#include "EnvironmentGenerator.h"
#include "Wall.h"
#include "randomizer.h"
#include "GameSettings.h"
#include "NPC.h"

namespace BaneAndBastion 
{
    std::vector<EnvironmentObject*> EnvironmentGenerator::GenerateForChunk(int cx, int cy) 
    {
        std::vector<EnvironmentObject*> objects;
        unsigned int seed = std::hash<int>{}(cx) ^ (std::hash<int>{}(cy) << 1);
        std::mt19937 gen(seed);
        std::uniform_int_distribution<int> dist(0, 100);

        for (int lx = 0; lx < GameSettings::ChunkSize; ++lx) 
        {
            for (int ly = 0; ly < GameSettings::ChunkSize; ++ly) 
            {
                float worldX = (cx * GameSettings::ChunkSize + lx) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
                float worldY = (cy * GameSettings::ChunkSize + ly) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;

                int roll = dist(gen);

                if (roll < 10) // 10% chance per wall
                {
                    objects.push_back(new Wall(worldX, worldY));
                }
            }
        }
        FE_APP_TRACE("Generator: Chunk [" + std::to_string(cx) + "," + std::to_string(cy) + "] generated with " + std::to_string(objects.size()) + " objects.");

        return objects;
    }
}