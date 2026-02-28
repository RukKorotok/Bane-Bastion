#pragma once

#include "pch.h"

#include <unordered_map>
#include "CollisionCategories.h"

namespace FalkonEngine {

/**
 * @brief Static registry that defines the interaction rules between collision categories.
 * Determines whether two objects should physically collide or pass through each other
 * based on their assigned CollisionCategory.
 */
class CollisionRegistry {
 public:
  /**
   * @brief Evaluates the collision mask between two categories.
   * @param a The category of the first object.
   * @param b The category of the second object.
   * @return True if a collision should be processed, false to ignore it.
   */
  static bool ShouldCollide(FalkonEngine::CollisionCategory a, FalkonEngine::CollisionCategory b) {
    using namespace FalkonEngine;

    // RULE 1: Self-collision ignore.
    // If categories are the same (e.g., Player vs Player or Enemy vs Enemy),
    // they pass through each other to prevent jittering or blocking teammates.
    if (a == b) return false;

    // RULE 2: Cross-category interaction.
    // In all other cases (e.g., Player vs Enemy, Projectile vs Wall),
    // the collision is valid and should be resolved by the PhysicsSystem.
    return true;
  }
};

}  // namespace FalkonEngine