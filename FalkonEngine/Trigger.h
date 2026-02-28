#pragma once

namespace FalkonEngine {

class ColliderComponent;

/**
 * @brief Represents a collision or overlap event between two entities.
 * Stores references to the participating ColliderComponents for physics resolution.
 */
struct Trigger {
 public:
  /**
   * @brief Constructs a Trigger event between two colliders.
   * @param newFirst Pointer to the first collider in the interaction.
   * @param newSecond Pointer to the second collider in the interaction.
   */
  Trigger(ColliderComponent* newFirst, ColliderComponent* newSecond) : first(newFirst), second(newSecond) {};

 private:
  ColliderComponent* first;   ///< Reference to the primary collider.
  ColliderComponent* second;  ///< Reference to the secondary collider.
};

}  // namespace FalkonEngine