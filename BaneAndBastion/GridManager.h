#pragma once

#include "SpriteColliderComponent.h"
#include "GridStructures.h"
#include "GameSettings.h"
#include <unordered_map>
#include "Vector.h"
#include "SubscriptionSystem.h"
#include "GameObject.h"
#include "CollisionCategories.h"

namespace BaneAndBastion
{
    class IEnvironmentGenerator;
    class NPCSpawner;

    /**
     * @brief Core manager for the tile-based grid system.
     * * Handles infinite chunk generation, coordinate conversion, and collision queries.
     * Acts as an Observer to respond to world events (like object removal).
     */
    class GridManager : public FalkonEngine::Observer
    {
    public:
        /**
         * @brief Constructs the GridManager with specific generation strategies.
         * @param generator Strategy for creating environment obstacles.
         * @param spawner Strategy for spawning NPCs within chunks.
         */
        GridManager(std::unique_ptr<IEnvironmentGenerator> generator, std::unique_ptr<NPCSpawner> spawner);

        virtual ~GridManager() = default;

        /**
         * @brief Converts grid coordinates (indices) to world-space coordinates.
         * @return Vector2Df representing the center of the tile in the world.
         */
        FalkonEngine::Vector2Df GridToWorld(int x, int y) const;

        /**
         * @brief Converts a single world-space coordinate to a grid index.
         */
        int WorldToGrid(float coord);

        /**
         * @brief Checks and creates chunks in the player's view area.
         * @param playerPos The current global position of the player.
         * @param viewDistance Radius of chunks to keep loaded (e.g., 1 = 3x3 grid around player).
         */
        void UpdateVisibleArea(const FalkonEngine::Vector2Df& playerPos, int viewDistance);

        /**
         * @brief Hot-swaps the environment generator strategy.
         */
        void ChangeGenerator(std::unique_ptr<IEnvironmentGenerator> newGen);

        /**
         * @brief Hot-swaps the NPC spawner strategy.
         */
        void ChangeNPCSpawner(std::unique_ptr<NPCSpawner> spawner);

        /**
         * @brief Retrieves a pointer to the cell at the given grid coordinates.
         * @return Pointer to Cell or nullptr if the chunk is not loaded.
         * @warning Not suitable for long-term caching if chunks can be unloaded.
         */
        Cell* GetCell(int worldX, int worldY);

        /**
         * @brief Marks a rectangular area in the grid as occupied by a specific category.
         * @param worldPos Center of the area in world coordinates.
         * @param size Dimensions of the area.
         * @param category The collision category to add to the blocking mask.
         */
        void OccupyArea(const FalkonEngine::Vector2Df& worldPos, const FalkonEngine::Vector2Df& size, FalkonEngine::CollisionCategory category);

        /**
         * @brief Removes a specific category from the blocking mask in a rectangular area.
         */
        void UnregisterEntity(const FalkonEngine::Vector2Df& pos, const FalkonEngine::Vector2Df& size, FalkonEngine::CollisionCategory category);

        /**
         * @brief Prints grid statistics and bitmasks to the console.
         */
        void DrawLogicDebug(const FalkonEngine::Vector2Df& playerPos);

        /**
         * @brief Draws a visual representation of the grid and collisions in the window.
         */
        void RenderDebug(sf::RenderWindow& window);

        /**
         * @brief Checks if a bounding box overlaps with any blocking tiles for a given category.
         * @return true if collision detected.
         */
        bool CheckGridCollision(const sf::FloatRect& bounds, FalkonEngine::CollisionCategory category);

        /**
         * @brief Handles events from the World (e.g., clearing grid data when an object is destroyed).
         */
        void OnNotify(const FalkonEngine::GameEvent& event) override;

        /**
         * @brief Finds the nearest tile that is not blocked for the given seeker.
         * Useful for resolving stuck entities or finding valid spawn points.
         */
        FalkonEngine::Vector2Df GetNearestPassablePoint(const FalkonEngine::Vector2Df& targetWorldPos, uint32_t seekerID);

    private:
        /// Container for all loaded chunks, indexed by their chunk-space coordinates.
        std::unordered_map<FalkonEngine::Vector2Di, Chunk, FalkonEngine::Vector2DiHasher> m_chunks;

        std::unique_ptr<IEnvironmentGenerator> m_generator;
        std::unique_ptr<NPCSpawner> m_npcSpawner;

        sf::FloatRect m_lastCheckBounds;

        /**
         * @brief Converts global grid coordinates to chunk-space coordinates.
         */
        FalkonEngine::Vector2Di WorldToChunk(int x, int y);

        /**
         * @brief Converts global grid coordinates to local indices within a chunk.
         */
        FalkonEngine::Vector2Di WorldToLocal(int x, int y);
    };
}