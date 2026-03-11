#include "pch.h"

#include "StatDefinition.h"

namespace BaneAndBastion {

// STATIC INITIALIZATION: Allocation of the static storage for stat definitions
std::unordered_map<std::string, StatDefinition> StatRegistry::m_registry;

//-----------------------------------------------------------------------------------------------------------
const StatDefinition& StatRegistry::GetDefinition(const std::string& statName) {
  // LOOKUP: Attempt to retrieve definition from the registry
  auto it = m_registry.find(statName);
  if (it != m_registry.end()) {
    return it->second;
  }

  // FALLBACK: Return a default definition to prevent runtime crashes if the stat is not registered
  static StatDefinition fallback;
  fallback.currentKey = statName;
  fallback.maxKey = statName;

  return fallback;
}

}  // namespace BaneAndBastion