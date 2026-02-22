#pragma once

#include "IEnvironmentGenerator.h"

namespace BaneAndBastion
{
    /**
     * @brief Procedural generator for forest-themed environment chunks.
     * * This class implements the IEnvironmentGenerator interface to populate
     * chunks with trees, walls, and other forest assets using deterministic randomness.
     */
    class ForestGenerator : public IEnvironmentGenerator
    {
    public:
        /**
         * @brief Initializes the generator and registers available assets.
         */
        ForestGenerator();

        /**
         * @brief Populates a specific chunk with environment objects.
         * * @param cx The X-coordinate of the chunk.
         * @param cy The Y-coordinate of the chunk.
         * @param safeGridPos Global grid position that should be kept clear of obstacles.
         * @param safeRadiusCells Square radius around the safe position to ignore generation.
         * @return A list of generated environment objects for the given chunk.
         */
        virtual std::vector<EnvironmentObject*> Generate(int cx, int cy,
            FalkonEngine::Vector2Di safeGridPos = { -999, -999 }, int safeRadiusCells = 0) override;

    };
}