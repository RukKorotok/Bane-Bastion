#include "pch.h"
#include "NPCSpawner.h"
#include "GameSettings.h"
#include "TransformComponent.h"
#include <random>
#include <string>

namespace BaneAndBastion
{
    std::vector<NPC*> NPCSpawner::GenerateNPCs(const FalkonEngine::Vector2Di& cp, const Chunk& chunk)
    {
        std::vector<NPC*> objects;

        // Deterministic seed based on chunk position
        std::mt19937 gen(std::hash<int>{}(cp.x) ^ (std::hash<int>{}(cp.y) << 1));

        for (int lx = 0; lx < GameSettings::ChunkSize; ++lx)
        {
            for (int ly = 0; ly < GameSettings::ChunkSize; ++ly)
            {
                // Select NPC type from registered weights
                std::type_index selected = GetRandomType(gen);
                if (selected == std::type_index(typeid(void)))
                {
                    continue;
                }

                // Overall spawn density check (5% chance per tile)
                std::uniform_real_distribution<float> chanceDist(0.0f, 100.0f);
                if (chanceDist(gen) > 5.0f)
                {
                    continue;
                }

                auto& entry = m_registry[selected];

                // Calculate centered world coordinates
                float wx = (cp.x * GameSettings::ChunkSize + lx) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;
                float wy = (cp.y * GameSettings::ChunkSize + ly) * GameSettings::PixelsPerUnit + GameSettings::PixelsPerUnit * 0.5f;

                // INITIALIZATION: Create a prototype on the first ever attempt to spawn this type
                if (!entry.isInitialized)
                {
                    entry.prototype = entry.factory(wx, wy);
                    entry.category = entry.prototype->GetCollisionCategory();
                    entry.isInitialized = true;
                }

                // PROTOTYPE DEPLOYMENT: If we have a pending prototype, try to place it
                if (entry.prototype != nullptr)
                {
                    // Verify if the cell is traversable for this specific NPC category
                    if (!chunk.cells[lx][ly].Blocks(entry.category))
                    {
                        auto transform = entry.prototype->GetGameObject()->GetComponent<FalkonEngine::TransformComponent>();
                        if (transform)
                        {
                            transform->SetWorldPosition({ wx, wy });
                        }

                        objects.push_back(entry.prototype);
                        entry.prototype = nullptr; // Prototype is now active in the world

                        FE_APP_TRACE("Prototype deployed to world at: " + std::to_string(wx) + ", " + std::to_string(wy));
                    }
                    continue; // Skip factory call since we were trying to place the prototype
                }

                // STANDARD SPAWN: Create a new instance if the cell is valid
                if (!chunk.cells[lx][ly].Blocks(entry.category))
                {
                    objects.push_back(entry.factory(wx, wy));
                }
            }
        }
        return objects;
    }

    std::type_index NPCSpawner::GetRandomType(std::mt19937& gen)
    {
        float totalWeight = 0.0f;
        for (auto const& [type, entry] : m_registry)
        {
            totalWeight += entry.weight;
        }

        std::uniform_real_distribution<float> dist(0.0f, totalWeight);
        float roll = dist(gen);
        float cumulative = 0.0f;

        for (auto const& [type, entry] : m_registry)
        {
            cumulative += entry.weight;
            if (roll < cumulative) return type;
        }

        return std::type_index(typeid(void));
    }
}