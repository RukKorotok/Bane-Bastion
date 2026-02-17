#include "pch.h"

#include "StatsComponent.h"
#include "GameObject.h"

namespace FalkonEngine
{
    //StatsComponent
    //--------------------------------------------------------------------------------------------------------
    StatsComponent::StatsComponent(GameObject* gameObject) : Component(gameObject)
    {
        FE_APP_TRACE("StatsComponent initialized for: " + p_gameObject->GetName());
    }
    //--------------------------------------------------------------------------------------------------------
    void StatsComponent::InitStats(std::initializer_list<std::pair<StatType, float>> stats)
    {
        for (const auto& [type, value] : stats)
        {
            m_stats[type] = value;

            FE_APP_TRACE("Stat '" + std::to_string((int)type) + "' initialized to " +
                std::to_string(value) + " on " + p_gameObject->GetName());
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void StatsComponent::SetStat(StatType type, float value)
    {
        if (m_stats.count(type) && m_stats[type] == value)
        {
            return;
        }

        m_stats[type] = value;

        try
        {
            FalkonEngine::GameEvent event;
            event.type = FalkonEngine::GameEventType::StatChanged;
            event.sender = this;
            event.actionID = static_cast<int>(type);
            event.value = value;
            event.entityID = p_gameObject->GetID();

            Notify(event);

            FE_APP_TRACE("Stat '" + std::to_string((int)type) + "' updated to " +
                std::to_string(value) + " on " + p_gameObject->GetName());
        }
        catch (const std::exception& e)
        {
            FE_CORE_ERROR("Exception during Stats Notification on '" +
                p_gameObject->GetName() + "': " + e.what());
        }
    }
    //--------------------------------------------------------------------------------------------------------
    float StatsComponent::ChangeStat(StatType type, float deltaValue)
    {
        float currentValue = GetStat(type);
        float newValue = currentValue + deltaValue;

        SetStat(type, newValue);

        return newValue;
    }
    //--------------------------------------------------------------------------------------------------------
    float StatsComponent::GetStat(StatType type) const
    {
        auto it = m_stats.find(type);
        if (it != m_stats.end())
        {
            return it->second;
        }
        return 0.0f;
    }
}