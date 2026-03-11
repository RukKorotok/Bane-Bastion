#pragma once

#include <string>
#include <unordered_map>

namespace BaneAndBastion {

/**
 * @brief Represents the data keys associated with a character statistic.
 */
struct StatDefinition {
  std::string currentKey;  ///< Key for the current value of the stat.
  std::string maxKey;      ///< Key for the maximum value of the stat.
};

/**
 * @brief Registry for global stat definitions, mapping stat names to their key pairs.
 */
class StatRegistry {
 public:
  /**
   * @brief Registers a new stat definition into the global registry.
   * @param statName Unique name of the statistic.
   * @param current The key identifier for the current value.
   * @param max The key identifier for the maximum value.
   */
  static void RegisterStat(const std::string& statName, const std::string& current, const std::string& max) {
    m_registry[statName] = {current, max};
  }

  /**
   * @brief Retrieves the definition for a specific statistic.
   * @param statName The name of the statistic to look up.
   * @return A constant reference to the StatDefinition.
   */
  static const StatDefinition& GetDefinition(const std::string& statName);

 private:
  static std::unordered_map<std::string, StatDefinition> m_registry;  ///< Internal storage for stat definitions.
};

}  // namespace BaneAndBastion