#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

#include "Collision.h"
#include "CollisionCategories.h"
#include "Component.h"
#include "PhysicsSystem.h"
#include "Trigger.h"

namespace FalkonEngine {

/**
 * @brief Base abstract class for all collision shapes in the engine.
 * Provides a framework for collision detection, trigger events, and
 * category-based filtering.
 */
class ColliderComponent : public Component {
 public:
  /**
   * @brief Constructs a ColliderComponent for a specific GameObject.
   * @param gameObject Pointer to the owner GameObject.
   */
  ColliderComponent(GameObject* gameObject);

  /**
   * @brief Pure virtual method to update collider bounds.
   */
  virtual void Update(float deltaTime) = 0;

  /**
   * @brief Pure virtual method for debug rendering of the collider area.
   */
  virtual void Render() = 0;

  /**
   * @brief Assigns a collision category for filtering interactions.
   * @param category The category mask to set.
   */
  void SetCategory(CollisionCategory category);

  /**
   * @return The current collision category of this collider.
   */
  CollisionCategory GetCategory() const;

  /**
   * @brief Sets whether this collider acts as a trigger (no physical response).
   * @param newIsTrigger True to enable trigger mode.
   */
  void SetTrigger(bool newIsTrigger);

  // -- Callbacks Subscription ---

  /** * @brief Adds a callback to be executed when a physical collision occurs.
   * @param onCollisionAction Functional callback receiving collision data.
   */
  void SubscribeCollision(std::function<void(Collision)> onCollisionAction);

  /** * @brief Removes a previously subscribed collision callback.
   */
  void UnsubscribeCollision(std::function<void(Collision)> onCollisionAction);

  /** * @brief Adds a callback to be executed when an entity enters this trigger.
   * @param onTriggerEnterAction Functional callback receiving trigger data.
   */
  void SubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction);

  /** * @brief Removes a previously subscribed trigger enter callback.
   */
  void UnsubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction);

  /** * @brief Adds a callback to be executed when an entity leaves this trigger.
   * @param onTriggerExitAction Functional callback receiving trigger data.
   */
  void SubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction);

  /** * @brief Removes a previously subscribed trigger exit callback.
   */
  void UnsubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction);

  friend class PhysicsSystem;

 protected:
  sf::FloatRect m_bounds;    ///< Axis-aligned bounding box (AABB) in world space.
  bool m_isTrigger = false;  ///< Flag indicating if the collider is a non-physical trigger.

  /**
   * @brief Internal dispatcher for collision events.
   */
  void OnCollision(Collision collision);

  /**
   * @brief Internal dispatcher for trigger enter events.
   */
  void OnTriggerEnter(Trigger trigger);

  /**
   * @brief Internal dispatcher for trigger exit events.
   */
  void OnTriggerExit(Trigger trigger);

  // Storage for functional listeners
  std::vector<std::function<void(Collision)>> p_onCollisionActions;
  std::vector<std::function<void(Trigger)>> p_onTriggerEnterActions;
  std::vector<std::function<void(Trigger)>> p_onTriggerExitActions;

  CollisionCategory m_cateogory = CollisionCategory::None;  ///< Bitmask for interaction filtering.
};

}  // namespace FalkonEngine