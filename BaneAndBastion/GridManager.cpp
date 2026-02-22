#include "pch.h"
#include "GridManager.h"
#include "MoveComponent.h"
#include "TransformComponent.h"
#include "GameScene.h"
#include "CellRegistry.h"
#include "Actor.h"
#include "IEnvironmentGenerator.h"
#include "NPCSpawner.h"

namespace BaneAndBastion
{
    GridManager::GridManager(std::unique_ptr<IEnvironmentGenerator> generator, std::unique_ptr<NPCSpawner> spawner)
        : m_generator(std::move(generator)), m_npcSpawner(std::move(spawner))
    {
        FE_CORE_ASSERT(m_generator != nullptr, "GridManager created with null generator!");

        // Subscribe to world events for automatic updates
        auto activeScene = FalkonEngine::Scene::GetActive();
        if (activeScene && activeScene->GetWorld())
        {
            activeScene->GetWorld()->Subscribe(this);
            FE_CORE_INFO("GridManager: Successfully initialized and subscribed to World.");
        }
        else
        {
            FE_CORE_ERROR("GridManager: Failed to subscribe to World. Active scene or world is null!");
        }
    }

    FalkonEngine::Vector2Df GridManager::GridToWorld(int x, int y) const
    {
        float ppu = static_cast<float>(GameSettings::PixelsPerUnit);
        return { static_cast<float>(x) * ppu + (ppu / 2.0f),
                static_cast<float>(y) * ppu + (ppu / 2.0f) };
    }

    int GridManager::WorldToGrid(float coord)
    {
        return static_cast<int>(std::floor(coord / GameSettings::PixelsPerUnit));
    }

    FalkonEngine::Vector2Di GridManager::WorldToChunk(int x, int y)
    {
        float size = static_cast<float>(GameSettings::ChunkSize);
        return FalkonEngine::Vector2Di(static_cast<int>(std::floor(static_cast<float>(x) / size)),
            static_cast<int>(std::floor(static_cast<float>(y) / size)));
    }

    FalkonEngine::Vector2Di GridManager::WorldToLocal(int x, int y) {
        int size = GameSettings::ChunkSize;
        int lx = x % size;
        int ly = y % size;
        if (lx < 0) lx += size;
        if (ly < 0) ly += size;
        return FalkonEngine::Vector2Di(lx, ly);
    }

    void GridManager::UpdateVisibleArea(const FalkonEngine::Vector2Df& playerPos, int viewDistance)
    {
        int gridX = WorldToGrid(playerPos.x);
        int gridY = WorldToGrid(playerPos.y);
        FalkonEngine::Vector2Di center = WorldToChunk(gridX, gridY);

        int safeRadius = 2; // Radius around player to keep clear of obstacles
        FalkonEngine::Vector2Di safePos(gridX, gridY);

        // Iterate through chunks within view distance
        for (int x = center.x - viewDistance; x <= center.x + viewDistance; ++x)
        {
            for (int y = center.y - viewDistance; y <= center.y + viewDistance; ++y)
            {
                FalkonEngine::Vector2Di pos(x, y);
                if (m_chunks.find(pos) == m_chunks.end())
                {
                    FE_APP_TRACE("GridManager: Generating new chunk at [" + std::to_string(x) + "," + std::to_string(y) + "]");
                    m_chunks[pos] = Chunk();

                    try
                    {
                        // 1. Generate environment objects (Static)
                        auto envObjects = m_generator->Generate(pos.x, pos.y, safePos, safeRadius);

                        if (auto* scene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive())) {
                            for (auto* env : envObjects)
                            {
                                scene->AddToChunk(pos, env);

                                // Sync environment collision with the grid cells
                                auto objPos = env->GetGameObject()->GetComponent<FalkonEngine::TransformComponent>()->GetWorldPosition();
                                int gx = WorldToGrid(objPos.x);
                                int gy = WorldToGrid(objPos.y);
                                FalkonEngine::Vector2Di lp = WorldToLocal(gx, gy);

                                m_chunks[pos].cells[lp.x][lp.y].blockingMask |= static_cast<std::uint8_t>(env->GetCollisionCategory());
                            }

                            // 2. Generate NPCs (Dynamic)
                            if (m_npcSpawner)
                            {
                                auto spawnedNPCs = m_npcSpawner->GenerateNPCs(pos, m_chunks[pos]);
                                for (auto* npc : spawnedNPCs)
                                {
                                    scene->AddToChunk(pos, npc);
                                }
                            }
                        }
                    }
                    catch (const std::exception& e)
                    {
                        FE_CORE_ERROR("GridManager: Failed to generate chunk. Error: " + std::string(e.what()));
                    }
                }
            }
        }
    }

    void GridManager::ChangeGenerator(std::unique_ptr<IEnvironmentGenerator> newGen)
    {
        if (newGen)
        {
            m_generator = std::move(newGen);
            FE_APP_INFO("GridManager: Generator strategy has been changed.");
            return;
        }
        FE_CORE_WARN("Attempted to change to a null generator!");
    }

    void GridManager::ChangeNPCSpawner(std::unique_ptr<NPCSpawner> spawner)
    {
        m_npcSpawner = std::move(spawner);
    }

    Cell* GridManager::GetCell(int x, int y)
    {
        FalkonEngine::Vector2Di cp = WorldToChunk(x, y);

        // Simple caching mechanism for the last accessed chunk
        static FalkonEngine::Vector2Di lastChunkPos = { -999, -999 };
        static Chunk* lastChunk = nullptr;

        if (cp != lastChunkPos)
        {
            auto it = m_chunks.find(cp);
            if (it == m_chunks.end()) return nullptr;

            lastChunk = &it->second;
            lastChunkPos = cp;
        }
        FalkonEngine::Vector2Di lp = WorldToLocal(x, y);
        return &lastChunk->cells[lp.x][lp.y];
    }

    bool GridManager::CheckGridCollision(const sf::FloatRect& bounds, FalkonEngine::CollisionCategory category)
    {
        float epsilon = 0.1f;
        int left = WorldToGrid(bounds.left + epsilon);
        int right = WorldToGrid(bounds.left + bounds.width - epsilon);
        int top = WorldToGrid(bounds.top + epsilon);
        int bottom = WorldToGrid(bounds.top + bounds.height - epsilon);

        for (int x = left; x <= right; ++x) {
            for (int y = top; y <= bottom; ++y) {
                Cell* cell = GetCell(x, y);
                if (cell && cell->Blocks(category)) return true;
            }
        }
        return false;
    }

    void GridManager::OccupyArea(const FalkonEngine::Vector2Df& worldPos, const FalkonEngine::Vector2Df& size, FalkonEngine::CollisionCategory category)
    {
        int left = WorldToGrid(worldPos.x - size.x / 2.0f);
        int right = WorldToGrid(worldPos.x + size.x / 2.0f - 0.1f);
        int top = WorldToGrid(worldPos.y - size.y / 2.0f);
        int bottom = WorldToGrid(worldPos.y + size.y / 2.0f - 0.1f);

        for (int x = left; x <= right; ++x) {
            for (int y = top; y <= bottom; ++y) {
                if (Cell* cell = GetCell(x, y))
                {
                    cell->blockingMask |= static_cast<std::uint8_t>(category);
                }
            }
        }
    }

    void GridManager::UnregisterEntity(const FalkonEngine::Vector2Df& pos, const FalkonEngine::Vector2Df& size, FalkonEngine::CollisionCategory category)
    {
        int L = WorldToGrid(pos.x - size.x / 2.0f);
        int R = WorldToGrid(pos.x + size.x / 2.0f - 0.1f);
        int T = WorldToGrid(pos.y - size.y / 2.0f);
        int B = WorldToGrid(pos.y + size.y / 2.0f - 0.1f);

        for (int x = L; x <= R; ++x)
        {
            for (int y = T; y <= B; ++y)
            {
                if (Cell* cell = GetCell(x, y))
                {
                    cell->blockingMask &= ~static_cast<std::uint8_t>(category);
                }
            }
        }
    }

    void GridManager::OnNotify(const FalkonEngine::GameEvent& event)
    {
        // Cleanup grid data when objects are destroyed
        if (event.type == FalkonEngine::GameEventType::ObjectRemoved && event.object)
        {
            // Implementation of dynamic unregistration here
        }
    }

    void GridManager::DrawLogicDebug(const FalkonEngine::Vector2Df& playerPos)
    {
        static sf::Clock debugClock;
        if (debugClock.getElapsedTime().asSeconds() > 0.5f)
        {
            std::system("cls");
            std::cout << "------- GRID LOGIC DEBUG (BITMASKS) -------" << std::endl;
            // Console output for occupied cells and masks...
            debugClock.restart();
        }
    }

    void GridManager::RenderDebug(sf::RenderWindow& window)
    {
        // Drawing visual grid boundaries and occupied cells with color coding
        // Blue = Static environment, Red = Dynamic/Other obstacles
    }

    FalkonEngine::Vector2Df GridManager::GetNearestPassablePoint(const FalkonEngine::Vector2Df& targetWorldPos, uint32_t seekerID)
    {
        int startX = WorldToGrid(targetWorldPos.x);
        int startY = WorldToGrid(targetWorldPos.y);

        // Radial search for the closest non-blocking tile
        for (int radius = 0; radius <= 20; ++radius)
        {
            for (int x = -radius; x <= radius; ++x)
            {
                for (int y = -radius; y <= radius; ++y)
                {
                    if (radius == 0 || std::abs(x) == radius || std::abs(y) == radius)
                    {
                        if (Cell* cell = GetCell(startX + x, startY + y))
                        {
                            if (!cell->Blocks(FalkonEngine::CollisionCategory::Enemy))
                            {
                                return GridToWorld(startX + x, startY + y);
                            }
                        }
                    }
                }
            }
        }
        return targetWorldPos;
    }
}