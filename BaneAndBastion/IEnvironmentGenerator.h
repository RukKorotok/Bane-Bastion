#pragma once

#include <vector>
#include <map>
#include <typeindex>
#include <functional>
#include <random>
#include "EnvironmentObject.h"
#include "GameSettings.h"
#include "CollisionCategories.h"

namespace BaneAndBastion
{
    /**
     * @brief Base interface for procedural environment generation.
     * * Provides a factory-based system for registering assets with specific
     * spawn weights and generating them within world chunks.
     */
    class IEnvironmentGenerator
    {
    public:
        /**
         * @brief Virtual destructor to ensure proper cleanup of derived generators.
         */
        virtual ~IEnvironmentGenerator() = default;

        /**
         * @brief Pure virtual method to generate environment objects for a specific chunk.
         * * @param cx Chunk X coordinate.
         * @param cy Chunk Y coordinate.
         * @param safeGridPos Global grid position to be kept clear of obstacles (e.g., spawn point).
         * @param safeRadiusCells Square radius in cells around the safe position to ignore generation.
         * @return A vector of pointers to the generated EnvironmentObjects.
         */
        virtual std::vector<EnvironmentObject*> Generate(int cx, int cy,
            FalkonEngine::Vector2Di safeGridPos = { -999, -999 }, int safeRadiusCells = 0) = 0;

    protected:
        /**
         * @brief Registers an asset type for the generation pool.
         * * @tparam T The class of the environment object (must derive from EnvironmentObject).
         * @param weight Spawn probability weight (usually between 0.0 and 1.0).
         */
        template<typename T>
        void RegisterAsset(float weight)
        {
            auto idx = std::type_index(typeid(T));
            m_weights[idx] = weight;
            m_factories[idx] = [](float x, float y) -> EnvironmentObject*
                {
                    return new T(FalkonEngine::Vector2Df(x, y));
                };
        }

        /**
         * @brief Selects a random asset type based on registered weights.
         * * @param gen The random number generator to use.
         * @return type_index of the selected asset or typeid(void) if no asset was chosen.
         */
        std::type_index GetRandomType(std::mt19937& gen);

        /// Map of asset types to their spawn probability weights.
        std::map<std::type_index, float> m_weights;

        /// Map of asset types to their respective factory functions for instantiation.
        std::map<std::type_index, std::function<EnvironmentObject* (float, float)>> m_factories;
    };
}