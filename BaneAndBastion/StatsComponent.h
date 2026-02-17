#pragma once

#include "Component.h"
#include "Stats.h"
#include <map>
#include <initializer_list>
#include <utility>

namespace FalkonEngine
{
    class StatsComponent : public Component
    {
    public:
        StatsComponent(GameObject* gameObject);

        void InitStats(std::initializer_list<std::pair<StatType, float>> stats);

        //** Set value and send message */
        void SetStat(StatType type, float value);
        float ChangeStat(StatType type, float deltaValue);

        float GetStat(StatType type) const;

        void Update(float deltaTime) override {}
        void Render() override {}

    private:
        std::map<StatType, float> m_stats;
    };
}