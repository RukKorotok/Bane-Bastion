#pragma once
#include <cstdint>

namespace FalkonEngine {

/**
 * @brief Bitwise enum for physical layers and collision filtering.
 * Each value represents a single bit, allowing for up to 8 distinct layers
 * within a std::uint8_t.
 */
enum CollisionCategory : std::uint8_t {
  None = 0,             ///< No physical presence.
  Player = 1 << 0,      ///< The player entity (Bit 0: 00000001)
  Enemy = 1 << 1,       ///< Enemy entities (Bit 1: 00000010)
  Projectile = 1 << 2,  ///< Bullets, arrows, etc. (Bit 2: 00000100)
  Flying = 1 << 3,      ///< Units that ignore certain ground obstacles (Bit 3: 00001000)
  Wall = 1 << 4,        ///< Static environment obstacles (Bit 4: 00010000)
  All = 0xFF            ///< Matches every layer (Binary: 11111111)
};

}  // namespace FalkonEngine