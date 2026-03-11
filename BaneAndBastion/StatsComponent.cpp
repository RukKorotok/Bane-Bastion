#include "pch.h"
#include "StatsComponent.h"
#include "GameObject.h"
#include "StatDefinition.h"

namespace BaneAndBastion {

//------------------------------------------------------------------------------------------------------------
StatsComponent::StatsComponent(FalkonEngine::GameObject* gameObject) : Component(gameObject) {
  FE_APP_TRACE("StatsComponent initialized for: " + p_gameObject->GetName());
}

//------------------------------------------------------------------------------------------------------------
void StatsComponent::InitStats(std::initializer_list<std::pair<std::string, float>> stats) {
  // Batch initialization of stats without triggering individual notifications
  for (const auto& [type, value] : stats) {
    m_stats[type] = value;
    FE_APP_TRACE("Stat '" + type + "' initialized to " + std::to_string(value) + " on " + p_gameObject->GetName());
  }
}

//------------------------------------------------------------------------------------------------------------
void StatsComponent::SetStat(std::string type, float value) {
  // Optimization: prevent redundant updates and event broadcasting if the value hasn't changed

    auto regKeys = StatRegistry::GetDefinition(type);

  if (m_stats.count(regKeys.currentKey) && m_stats[regKeys.currentKey] == value) {
    return;
  }

  m_stats[regKeys.currentKey] = value;

  // Create and dispatch a GameEvent to notify listeners (UI, Health system, etc.)
  FalkonEngine::GameEvent event;
  event.type = FalkonEngine::GameEventType::StatChanged;
  event.sender = this;
  event.input[regKeys.currentKey] = value;
  auto max = m_stats.find(regKeys.maxKey)->second;
  event.input[regKeys.maxKey] = max;
  event.entityID = p_gameObject->GetID();

  // Notify all registered Observers of the change
  Notify(event);

  FE_APP_TRACE("Stat '" + type + "' updated to " + std::to_string(value) + " on " + p_gameObject->GetName());
}

//------------------------------------------------------------------------------------------------------------
float StatsComponent::ChangeStat(std::string type, float deltaValue) {
  // Wrapper for relative modification; retrieves current value before applying delta

  float currentValue = GetStat(type);
  float newValue = currentValue + deltaValue;

  SetStat(type, newValue);
  return newValue;
}

//------------------------------------------------------------------------------------------------------------
float StatsComponent::GetStat(std::string type) const {
  // Safe lookup: returns current value if found, otherwise defaults to 0.0f
  auto regKeys = StatRegistry::GetDefinition(type);

  auto it = m_stats.find(regKeys.currentKey);
  if (it != m_stats.end()) {
    return it->second;
  }
  return 0.0f;
}

//------------------------------------------------------------------------------------------------------------
std::pair<float, float> StatsComponent::GetStatPair(const std::string& statName) const {
  // Use StatRegistry to look up the associated 'current' and 'max' keys
  const auto& def = StatRegistry::GetDefinition(statName);

  float current = GetStat(def.currentKey);
  float max = GetStat(def.maxKey);

  return {current, max};
}

}  // namespace FalkonEngine