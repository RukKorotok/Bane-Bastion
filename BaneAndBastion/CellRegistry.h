#pragma once

#include <unordered_map>
#include "GridStructures.h"

namespace BaneAndBastion
{
    /**
     * @brief Singleton registry that defines movement constraints for each cell type.
     * * Maps CellType (visual/logical type) to a bitmask of collision categories
     * that are BLOCKED by that cell.
     */
    class CellRegistry
    {
    public:
        /**
         * @brief Access the singleton instance of the registry.
         */
        static CellRegistry& Get()
        {
            static CellRegistry instance;
            return instance;
        }

        /**
         * @brief Returns the collision mask for a specific cell type.
         * @param type The logical type of the cell (Empty, Wall, Water, etc.).
         * @return A bitmask representing which categories cannot pass through this cell.
         */
        std::uint8_t GetMask(CellType type) const
        {
            auto it = m_props.find(type);
            return (it != m_props.end()) ? it->second : 0;
        }

    private:
        /**
         * @brief Private constructor to initialize collision rules.
         * * Defines which entities (Player, Enemy, Projectiles, etc.) are blocked
         * by specific environmental features.
         */
        CellRegistry()
        {
            // Combined mask for all ground-based entities
            std::uint8_t AllWalking = CollisionCategory::Player | CollisionCategory::Enemy;

            // Empty: No blocks
            m_props[CellType::Empty] = FalkonEngine::CollisionCategory::None;

            // Wall: Solid barrier for everyone
            m_props[CellType::Wall] = FalkonEngine::CollisionCategory::All;

            // Obstacle (e.g. Bushes): Blocks movement but allows projectiles/flying units
            m_props[CellType::Obstacle] = AllWalking | FalkonEngine::Flying;

            // Water: Only blocks ground-based movement
            m_props[CellType::Water] = AllWalking;

            // Abyss: Blocks ground movement and projectiles, allows flying units
            m_props[CellType::Abyss] = AllWalking | FalkonEngine::Projectile;
        }

        /// Map storing the relationship between cell types and their blocking masks.
        std::unordered_map<CellType, std::uint8_t> m_props;
    };
}