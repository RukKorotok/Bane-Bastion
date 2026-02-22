#pragma once
#include "CollisionCategories.h"
#include "GameSettings.h"

namespace BaneAndBastion {
    /**
     * @brief Logical types of environment tiles.
     */
    enum class CellType : std::uint8_t
    {
        Empty = 0,    ///< Walkable ground.
        Wall = 1,     ///< Solid impassable wall.
        Obstacle = 2, ///< Small obstacle (e.g., bush, rock).
        Water = 3,    ///< Liquid surface (blocks non-flying units).
        Abyss = 4     ///< Deep pit (blocks ground units and projectiles).
    };

    /**
     * @brief Represents a single tile in the grid.
     * * Stores its type and a bitmask of categories it blocks.
     */
    struct Cell
    {
        CellType type = CellType::Empty;

        /// Bitmask of CollisionCategory flags that cannot pass through this cell.
        std::uint8_t blockingMask = FalkonEngine::CollisionCategory::None;

        /**
         * @brief Efficiently checks if a specific category is blocked by this cell.
         * @param category The category to check (Player, Enemy, Projectile, etc.).
         * @return true if the category is blocked.
         */
        bool Blocks(FalkonEngine::CollisionCategory category) const
        {
            return (blockingMask & static_cast<std::uint8_t>(category)) != 0;
        }
    };

    /**
     * @brief A square block of cells for chunk-based world management.
     * * Size is determined by GameSettings::ChunkSize.
     */
    struct Chunk
    {
        /// 2D array of cells stored contiguously in memory.
        Cell cells[GameSettings::ChunkSize][GameSettings::ChunkSize];
    };
}