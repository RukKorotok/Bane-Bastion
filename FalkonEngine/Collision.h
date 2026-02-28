#pragma once

#include <SFML/Graphics.hpp>
#include "Vector.h"

namespace FalkonEngine {

class ColliderComponent;

/**
 * @brief Data structure containing details about a collision event.
 * Encapsulates the involved colliders and the geometry of their intersection.
 */
struct Collision {
 public:
  /**
   * @brief Constructs a collision record.
   * @param newFirst Pointer to the first collider involved.
   * @param newSecond Pointer to the second collider involved.
   * @param newCollisionRect The overlapping rectangle area between the two colliders.
   */
  Collision(ColliderComponent* newFirst, ColliderComponent* newSecond, sf::FloatRect newCollisionRect);

  /**
   * @brief Calculates the vector required to push the first object out of the second.
   * Based on the smallest axis of the intersection rectangle.
   * @return A normalized or weighted direction vector for the knockback effect.
   */
  Vector2Df GetKnockbackDirection() const;

  /** @return Pointer to the first participant of the collision. */
  ColliderComponent* GetFirst() const;

  /** @return Pointer to the second participant of the collision. */
  ColliderComponent* GetSecond() const;

  /** @return The area (AABB) where the two colliders overlap. */
  sf::FloatRect GetRect() const;

 private:
  ColliderComponent* m_first;     ///< First collider (usually the one being moved).
  ColliderComponent* m_second;    ///< Second collider (usually the obstacle).
  sf::FloatRect m_collisionRect;  ///< Intersection geometry in world space.
};

}  // namespace FalkonEngine