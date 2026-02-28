#pragma once

#include "SubScriptionSystem.h"

namespace FalkonEngine {

class GameObject;

/**
 * @brief Base abstract class for all components in the FalkonEngine.
 * Components define the behavior and properties of GameObjects.
 * Inherits from Observable to support the engine's event-driven architecture.
 */
class Component : public Observable {
 public:
  /**
   * @brief Constructs a component and binds it to a specific GameObject.
   * @param gameObject Pointer to the owner GameObject.
   */
  Component(GameObject* gameObject);

  /**
   * @brief Virtual destructor to ensure proper cleanup of derived components.
   */
  virtual ~Component();

  // -- (Core functions) --

  /** * @brief Method called every frame to update component logic.
   * @param deltaTime The duration of the current frame in seconds.
   */
  virtual void Update(float deltaTime) = 0;

  /** * @brief Method called every frame to handle component-specific rendering.
   */
  virtual void Render() = 0;

  /** * @brief Returns the pointer to the owner GameObject.
   * @return GameObject* The parent object this component is attached to.
   */
  GameObject* GetGameObject();

 protected:
  GameObject* p_gameObject;  ///< Pointer to the parent GameObject (owner).
};

}  // namespace FalkonEngine