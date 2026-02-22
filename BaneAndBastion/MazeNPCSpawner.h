#pragma once

#include "NPCSpawner.h"

namespace BaneAndBastion
{
    /**
     * @brief Specialized NPC spawner for maze-like environments.
     * * This class configures the spawn registry specifically for dungeons
     * and mazes, ensuring appropriate enemy types and densities.
     */
    class MazeNPCSpawner : public NPCSpawner
    {
    public:
        /**
         * @brief Initializes the maze spawner and registers dungeon-specific NPCs.
         */
        MazeNPCSpawner()
        {
            // Register basic NPCs with a 10% weight for dungeon corridors
            RegisterAsset<NPC>(0.10f);
        }

    };
}