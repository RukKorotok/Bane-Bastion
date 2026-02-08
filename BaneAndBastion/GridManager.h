#pragma once

#include "SpriteColliderComponent.h"
#include "GridStructures.h"
#include "GameSettings.h"
#include <unordered_map>
#include "Vector.h"
#include "SubscriptionSystem.h"
#include "GameObject.h"
#include "EnvironmentGenerator.h"

namespace BaneAndBastion
{
    struct ChunkLoadedMessage
    {
        int x = 0;
        int y = 0;
    };

    class GridManager : public FalkonEngine::Observer
    {
    public:
        GridManager();
        FalkonEngine::Vector2Df GridToWorld(int x, int y) const;
        int WorldToGrid(float coord);
        /**
         * @brief Check and create chunks in view area.
         * @param playerPos global position.
         * @param viewDistance (1 =  3x3 chunk).
         */
        void UpdateVisibleArea(const FalkonEngine::Vector2Df& playerPos, int viewDistance);

        /**
         * @brief get cell in world coord.
         * @return ptr on Cell or @p nullptr.
         * @warning Not suitable for caching if the chunk can be modified.
         */
        Cell* GetCell(int worldX, int worldY);

        /**
        * @brief Mark occupy cell by entity.
        * @param worldPos center position in world coord.
        * @param size ( 2.0f for ogr).
        */
        void OccupyArea(const FalkonEngine::Vector2Df& worldPos, const FalkonEngine::Vector2Df& size, std::uint32_t entityID);

        void UnregisterEntity(uint32_t id, const FalkonEngine::Vector2Df& pos, FalkonEngine::SpriteColliderComponent* col);

        // --- (Debug) ---
        void DrawLogicDebug(const FalkonEngine::Vector2Df& playerPos);
        void RenderDebug(sf::RenderWindow& window);

        bool CheckGridCollision(const sf::FloatRect& bounds, uint32_t seekerID);

        void OnNotify(const FalkonEngine::GameEvent& event) override;

    private:
        std::unordered_map<FalkonEngine::Vector2Di, Chunk, FalkonEngine::Vector2DiHasher> m_chunks;

        FalkonEngine::Vector2Di WorldToChunk(int x, int y);
        FalkonEngine::Vector2Di WorldToLocal(int x, int y);
        sf::FloatRect m_lastCheckBounds;

    };
}
