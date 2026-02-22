#pragma once

#include <vector>
#include <map>
#include <typeindex>
#include <functional>
#include <random>
#include "GameSettings.h"
#include "CollisionCategories.h"
#include "NPC.h"

namespace BaneAndBastion
{
    /**
     * @brief Data structure for managing NPC spawn logic.
     * * Contains the factory function for instantiation, spawn probability,
     * and a prototype object for lazy initialization.
     */
    struct SpawnEntry
    {
        std::function<NPC* (float, float)> factory; ///< Function to create a new NPC instance.
        float weight;                               ///< Probability weight for spawning.
        FalkonEngine::CollisionCategory category;   ///< Collision category assigned to the NPC.
        NPC* prototype = nullptr;                   ///< Lazy-initialized prototype object.
        bool isInitialized = false;                 ///< Flag to check if the entry is ready.
    };

    /**
     * @brief Base class for NPC spawning logic within chunks.
     * * Uses a registry-based system to randomly select and instantiate NPC units
     * based on predefined weights.
     */
    class NPCSpawner
    {
    public:
        /**
         * @brief Virtual destructor for proper cleanup of derived spawner types.
         */
        virtual ~NPCSpawner() = default;

        /**
         * @brief Generates a list of NPCs for a specific chunk.
         * * @param cp Chunk position in grid coordinates.
         * @param chunk Reference to the chunk data for placement context.
         * @return A vector of pointers to newly created NPC instances.
         */
        virtual std::vector<NPC*> GenerateNPCs(const FalkonEngine::Vector2Di& cp, const Chunk& chunk);

    protected:
        /**
         * @brief Registers an NPC type into the spawn registry.
         * * @tparam T The specific NPC class (must derive from NPC).
         * @param weight Spawn probability weight.
         */
        template<typename T>
        void RegisterAsset(float weight)
        {
            auto idx = std::type_index(typeid(T));

            m_registry[idx] = {
                [](float x, float y) -> NPC*
                { return new T(FalkonEngine::Vector2Df(x, y)); },
                weight,
                FalkonEngine::CollisionCategory::None,
                nullptr,
                false
            };
        }

    private:
        /**
         * @brief Selects a random NPC type index from the registry using a distribution.
         * * @param gen The random number generator instance.
         * @return The type_index of the selected NPC.
         */
        std::type_index GetRandomType(std::mt19937& gen);

        /// Map of registered NPC types and their spawn settings.
        std::map<std::type_index, SpawnEntry> m_registry;
    };
}