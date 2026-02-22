#include "pch.h"
#include "IEnvironmentGenerator.h"
#include "Wall.h"
#include "randomizer.h"
#include "GameSettings.h"
#include "NPC.h"

namespace BaneAndBastion
{
    std::type_index IEnvironmentGenerator::GetRandomType(std::mt19937& gen)
    {
        // Define a distribution between 0.0 and 1.0
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        float roll = dist(gen);
        float cumulative = 0.0f;

        // Iterate through registered assets to check which one the "roll" falls into
        for (auto const& [type, weight] : m_weights)
        {
            cumulative += weight;
            if (roll < cumulative)
            {
                return type; // Asset selected
            }
        }

        // Return void type if no asset was selected (spawn empty space)
        return std::type_index(typeid(void));
    }
}