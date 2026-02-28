#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "ColliderComponent.h"
#include "CollisionRegistry.h"
#include "RigidbodyComponent.h"
#include "Vector.h"

namespace FalkonEngine {

/**
 * @brief Singleton system responsible for collision detection and resolution.
 * Processes physical interactions between Colliders at a fixed time step
 * and manages trigger event states.
 */
class PhysicsSystem {
 public:
  /** @return The global instance of the PhysicsSystem. */
  static PhysicsSystem* Instance();

  /**
   * @brief Performs collision detection pass.
   * Iterates through all registered colliders, checks for overlaps based on
   * CollisionRegistry rules, and resolves physical penetrations.
   */
  void Update();

  /** @return The constant time step used for physics calculations (e.g., 0.02s for 50Hz). */
  float GetFixedDeltaTime() const;

  /**
   * @brief Registers a new collider to be tracked by the physics engine.
   * @param collider Pointer to the component to add.
   */
  void Subscribe(ColliderComponent* collider);

  /**
   * @brief Removes a collider from the physics simulation.
   * Usually called when a GameObject is destroyed.
   */
  void Unsubscribe(ColliderComponent* collider);

 private:
  PhysicsSystem() {}
  ~PhysicsSystem() {}

  // Singleton protection: Ensure only one physics world exists.
  PhysicsSystem(PhysicsSystem const&) = delete;
  PhysicsSystem& operator=(PhysicsSystem const&) = delete;

  /// Registry of all active colliders in the current scene.
  std::vector<ColliderComponent*> m_colliders;

  /** * @brief Tracks currently active trigger overlaps.
   * Used to determine when an object has just entered or just left a trigger zone.
   */
  std::map<ColliderComponent*, ColliderComponent*> m_triggersEnteredPair;

  /// The interval (in seconds) between physics updates. 0.02f = 50 updates per second.
  float m_fixedDeltaTime = 0.02f;
};

}  // namespace FalkonEngine