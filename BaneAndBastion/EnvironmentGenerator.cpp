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

        for (int lx = 0; lx < GameSettings::ChunkSize; ++lx) 
        {
            for (int ly = 0; ly < GameSettings::ChunkSize; ++ly) 
            {
                float worldX = (cx * GameSettings::ChunkSize + lx) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
                float worldY = (cy * GameSettings::ChunkSize + ly) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;

                int roll = random<int>(0, 100);

                if (roll < 10) // 10% chance per wall
                {
                    objects.push_back(new Wall(worldX, worldY));
                }
            }
        }
        return objects;
    }
}