#pragma once

#include "IEnvironmentGenerator.h"

namespace BaneAndBastion
{
    /**
     * @brief Procedural generator for maze-like environment chunks.
     * * This class implements the IEnvironmentGenerator interface to create structured
     * layouts, such as corridors and rooms, using deterministic algorithms.
     */
    class MazeGenerator : public IEnvironmentGenerator
    {
    public:
        /**
         * @brief Initializes the maze generator and registers structural assets.
         */
        MazeGenerator();

        /**
         * @brief Generates a maze structure for a specific chunk.
         * * @param cx Chunk X coordinate.
         * @param cy Chunk Y coordinate.
         * @param safeGridPos Global grid position to remain unobstructed.
         * @param safeRadiusCells Radius in cells around the safe position to keep clear.
         * @return A vector of pointers to environment objects forming the maze.
         */
        virtual std::vector<EnvironmentObject*> Generate(int cx, int cy,
            FalkonEngine::Vector2Di safeGridPos = { -999, -999 }, int safeRadiusCells = 0) override;

    };
}