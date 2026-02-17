#include "pch.h"
#include "GridManager.h"
#include "MoveComponent.h"
#include "TransformComponent.h"
#include "GameScene.h"

namespace BaneAndBastion {
    //GridManager
    //--------------------------------------------------------------------------------------------------------
    GridManager::GridManager() 
    {
        auto activeScene = FalkonEngine::Scene::GetActive();
        if (activeScene && activeScene->GetWorld()) 
        {
            activeScene->GetWorld()->Subscribe(this);
        }
        else {
            FE_CORE_ERROR("GridManager: Failed to subscribe to World. Active scene or world is null!");
        }
    }
    //--------------------------------------------------------------------------------------------------------
    FalkonEngine::Vector2Df GridManager::GridToWorld(int x, int y) const
    {
        float ppu = static_cast<float>(GameSettings::PixelsPerUnit);
        return {static_cast<float>(x) * ppu + (ppu / 2.0f),
                static_cast<float>(y) * ppu + (ppu / 2.0f)
               };
    }
    //--------------------------------------------------------------------------------------------------------
    int GridManager::WorldToGrid(float coord) 
    {
        return static_cast<int>(std::floor(coord / GameSettings::PixelsPerUnit));
    }
    //--------------------------------------------------------------------------------------------------------
    FalkonEngine::Vector2Di GridManager::WorldToChunk(int x, int y) 
    {
        float size = static_cast<float>(GameSettings::ChunkSize);
        return FalkonEngine::Vector2Di(static_cast<int>(std::floor(static_cast<float>(x) / size)),
                                       static_cast<int>(std::floor(static_cast<float>(y) / size))
                                      );
    }
    //--------------------------------------------------------------------------------------------------------
    FalkonEngine::Vector2Di GridManager::WorldToLocal(int x, int y) {
        int size = GameSettings::ChunkSize;
        int lx = x % size;
        int ly = y % size;
        if (lx < 0) lx += size;
        if (ly < 0) ly += size;
        return FalkonEngine::Vector2Di(lx, ly);
    }
    //--------------------------------------------------------------------------------------------------------
    void GridManager::UpdateVisibleArea(const FalkonEngine::Vector2Df& playerPos, int viewDistance) 
    {
        int gridX = WorldToGrid(playerPos.x);
        int gridY = WorldToGrid(playerPos.y);
        FalkonEngine::Vector2Di center = WorldToChunk(gridX, gridY);

        for (int x = center.x - viewDistance; x <= center.x + viewDistance; ++x) 
        {
            for (int y = center.y - viewDistance; y <= center.y + viewDistance; ++y) 
            {
                FalkonEngine::Vector2Di pos(x, y);
                if (m_chunks.find(pos) == m_chunks.end()) 
                {
                    FE_APP_TRACE("GridManager: Generating new chunk at [" + std::to_string(x) + "," + std::to_string(y) + "]");

                    m_chunks[pos] = Chunk();

                    auto envObjects = EnvironmentGenerator::GenerateForChunk(pos.x, pos.y);

                    if (auto* scene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive())) 
                    {
                        for (auto* obj : envObjects) scene->AddToChunk(pos, obj);
                    }
                }
            }
        }
    }
    //--------------------------------------------------------------------------------------------------------
    Cell* GridManager::GetCell(int x, int y) 
    {
        FalkonEngine::Vector2Di cp = WorldToChunk(x, y);

        static FalkonEngine::Vector2Di lastChunkPos = { -999, -999 };
        static Chunk* lastChunk = nullptr;

        if (cp != lastChunkPos)
        {
            auto it = m_chunks.find(cp);

            if (it == m_chunks.end()) 
            {
                return nullptr;
            }
            lastChunk = &it->second;
            lastChunkPos = cp;
        }
        FalkonEngine::Vector2Di lp = WorldToLocal(x, y);
        return &lastChunk->cells[lp.x][lp.y];
    }
    //--------------------------------------------------------------------------------------------------------
    bool GridManager::CheckGridCollision(const sf::FloatRect& bounds, uint32_t seekerID) 
    {
        float epsilon = 0.1f; // For narrow passage
        int left = WorldToGrid(bounds.left + epsilon);
        int right = WorldToGrid(bounds.left + bounds.width - epsilon);
        int top = WorldToGrid(bounds.top + epsilon);
        int bottom = WorldToGrid(bounds.top + bounds.height - epsilon);

        for (int x = left; x <= right; ++x) {
            for (int y = top; y <= bottom; ++y) {
                Cell* cell = GetCell(x, y);
                // If there is no cell(edge ??of the map) or it is impassable
                if (cell == nullptr || !cell->CanBeOccupiedBy(seekerID))
                {
                    return true;
                }
            }
        }
        return false;
    }
    //--------------------------------------------------------------------------------------------------------
    void GridManager::OccupyArea(const FalkonEngine::Vector2Df& worldPos, const FalkonEngine::Vector2Df& size, std::uint32_t entityID) 
    {
        int left = WorldToGrid(worldPos.x - size.x / 2.0f);
        int right = WorldToGrid(worldPos.x + size.x / 2.0f - 0.1f);
        int top = WorldToGrid(worldPos.y - size.y / 2.0f);
        int bottom = WorldToGrid(worldPos.y + size.y / 2.0f - 0.1f);

        for (int x = left; x <= right; ++x) {
            for (int y = top; y <= bottom; ++y) {
                if (Cell* cell = GetCell(x, y)) cell->entityID = entityID;
            }
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void GridManager::UnregisterEntity(uint32_t id, const FalkonEngine::Vector2Df& pos, FalkonEngine::SpriteColliderComponent* col) 
    {
        float ppu = GameSettings::PixelsPerUnit;
        float halfW = (col ? col->GetBounds().width : ppu) / 2.0f;
        float halfH = (col ? col->GetBounds().height : ppu) / 2.0f;

        int L = WorldToGrid(pos.x - halfW);
        int R = WorldToGrid(pos.x + halfW - 0.1f);
        int T = WorldToGrid(pos.y - halfH);
        int B = WorldToGrid(pos.y + halfH - 0.1f);

        for (int x = L; x <= R; ++x) {
            for (int y = T; y <= B; ++y) {
                Cell* cell = GetCell(x, y);
                if (cell && cell->entityID == id) cell->entityID = 0;
            }
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void GridManager::OnNotify(const FalkonEngine::GameEvent& event) 
    {
        if (event.type == FalkonEngine::GameEventType::ObjectRemoved && event.object) 
        {
            auto col = event.object->GetComponent<FalkonEngine::SpriteColliderComponent>();
            auto tr = event.object->GetComponent<FalkonEngine::TransformComponent>();
            if (tr) UnregisterEntity(event.object->GetID(), tr->GetWorldPosition(), col);
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void GridManager::DrawLogicDebug(const FalkonEngine::Vector2Df& playerPos)
    {
        static sf::Clock debugClock;
        if (debugClock.getElapsedTime().asSeconds() > 0.5f) 
        {
            std::system("cls");

            std::cout << "------- ALL OCCUPIED CELLS -------" << std::endl;
            std::cout << "Total Chunks Loaded: " << m_chunks.size() << std::endl;
            std::cout << "Player at: [" << playerPos.x << ", " << playerPos.y << "]" << std::endl;
            std::cout << "----------------------------------" << std::endl;
            std::cout << "  CHUNK   |  LOCAL  |  GLOBAL |   ID   " << std::endl;
            std::cout << "----------|---------|---------|--------" << std::endl;

            int chunkSize = GameSettings::ChunkSize;

            for (auto& [chunkPos, chunk] : m_chunks) 
            {
                for (int lx = 0; lx < chunkSize; ++lx) 
                {
                    for (int ly = 0; ly < chunkSize; ++ly) 
                    {
                        uint32_t id = chunk.cells[lx][ly].entityID;
                        if (id != 0) 
                        {
                            int gx = chunkPos.x * chunkSize + lx;
                            int gy = chunkPos.y * chunkSize + ly;

                            printf("[%3d,%3d] | [%2d,%2d] | [%3d,%3d] | #%d\n",
                                chunkPos.x, chunkPos.y, lx, ly, gx, gy, id);
                        }
                    }
                }
            }
            std::cout << "----------------------------------" << std::endl;
            debugClock.restart();
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void GridManager::RenderDebug(sf::RenderWindow& window)
    {
        float ppu = GameSettings::PixelsPerUnit;
        float chunkSizePixels = GameSettings::ChunkSize * ppu;

        sf::RectangleShape tileRect(sf::Vector2f(ppu - 1.0f, ppu - 1.0f));
        sf::RectangleShape chunkBounds(sf::Vector2f(chunkSizePixels, chunkSizePixels));
        chunkBounds.setFillColor(sf::Color::Transparent);
        chunkBounds.setOutlineColor(sf::Color::Green);
        chunkBounds.setOutlineThickness(2.0f);

        for (auto const& [chunkPos, chunk] : m_chunks) 
        {
            float chunkWorldX = chunkPos.x * chunkSizePixels;
            float chunkWorldY = chunkPos.y * chunkSizePixels;
            // chunk frame
            chunkBounds.setPosition(chunkWorldX, chunkWorldY);
            window.draw(chunkBounds);
            // Cells(we draw only if they are occupied, so as not to overload the renderer)
            for (int x = 0; x < GameSettings::ChunkSize; ++x) 
            {
                for (int y = 0; y < GameSettings::ChunkSize; ++y) 
                {
                    if (chunk.cells[x][y].entityID != 0) 
                    {
                        tileRect.setPosition(chunkWorldX + (x * ppu), chunkWorldY + (y * ppu));
                        tileRect.setFillColor(sf::Color(255, 0, 0, 150));
                        window.draw(tileRect);
                    }
                }
            }
        }
    }
    //--------------------------------------------------------------------------------------------------------
    FalkonEngine::Vector2Df GridManager::GetNearestPassablePoint(const FalkonEngine::Vector2Df& targetWorldPos, uint32_t seekerID)
    {
        int startX = WorldToGrid(targetWorldPos.x);
        int startY = WorldToGrid(targetWorldPos.y);

        const int maxSearchRadius = 20;

        for (int radius = 0; radius <= maxSearchRadius; ++radius)
        {
            for (int x = -radius; x <= radius; ++x)
            {
                for (int y = -radius; y <= radius; ++y)
                {
                    if (radius == 0 || std::abs(x) == radius || std::abs(y) == radius)
                    {
                        int curX = startX + x;
                        int curY = startY + y;

                        Cell* cell = GetCell(curX, curY);

                        if (cell && cell->CanBeOccupiedBy(seekerID))
                        {
                            return GridToWorld(curX, curY);
                        }
                    }
                }
            }
        }
        return targetWorldPos;
    }
}