#pragma once

#include "Component.h"
#include "Stats.h"
#include <map>
#include <initializer_list>
#include <utility>

namespace FalkonEngine
{
    /**
     * @brief Component for managing numerical entity attributes (Health, Mana, Strength, etc.).
     * * Acts as a centralized data container for stats, allowing other systems
     * to modify or query entity properties via a standardized StatType enum.
     */
    class StatsComponent : public Component
    {
    public:
        /**
         * @brief Constructor for the Stats component.
         * @param gameObject Pointer to the owner GameObject.
         */
        StatsComponent(GameObject* gameObject);

        /**
         * @brief Batch initialization of stats using an initializer list.
         * Useful for setting starting attributes in a single call.
         * @param stats List of StatType and float value pairs.
         */
        void InitStats(std::initializer_list<std::pair<StatType, float>> stats);

        /**
         * @brief Sets a specific stat to a new value and triggers a notification.
         * @param type The category of the stat to modify.
         * @param value The new absolute value to store.
         */
        void SetStat(StatType type, float value);

        /**
         * @brief Adjusts a stat by a relative amount (addition or subtraction).
         * @param type The category of the stat to modify.
         * @param deltaValue The amount to add to the current value.
         * @return The updated value of the stat.
         */
        float ChangeStat(StatType type, float deltaValue);

        /**
         * @brief Retrieves the current value of a specific stat.
         * @param type The category of the stat to look up.
         * @return The current float value or 0.0f if the stat is not initialized.
         */
        float GetStat(StatType type) const;

        /** @brief Empty implementation; logic is driven by external setters/getters. */
        void Update(float deltaTime) override {}

        /** @brief Logic-only component; no visual representation. */
        void Render() override {}

    private:
        /**
         * @brief Internal storage mapping stat categories to their current values.
         */
        std::map<StatType, float> m_stats;
    };
}