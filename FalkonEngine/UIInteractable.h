#pragma once

#include "Component.h"
#include "SubscriptionSystem.h"

namespace FalkonEngine {

/**
 * @brief Base class for UI elements that respond to mouse or touch input.
 * Implements the Observer pattern to listen for global input events and
 * manages basic interaction states like Hover and Pressed.
 */
class UIInteractable : public Component, public Observer {
 public:
  /**
   * @brief Constructs a UIInteractable component and registers it as an input observer.
   * @param gameObject Pointer to the owner GameObject.
   */
  UIInteractable(GameObject* gameObject);

  /**
   * @brief Structural component; logic is handled via events, so Update is empty.
   */
  void Update(float deltaTime) override {}

  /**
   * @brief Structural component; visual representation is handled by specific UI renderers.
   */
  void Render() override {};

  /**
   * @brief Handles incoming game events (e.g., MouseMoved, MouseButtonPressed).
   * @param event The event data containing input coordinates or states.
   */
  void OnNotify(const GameEvent& event) override;

 protected:
  bool m_isHovered = false;  ///< Tracks if the mouse cursor is currently over the element.
  bool m_isPressed = false;  ///< Tracks if the element is currently being clicked/held.

  /**
   * @brief Checks if a given point (usually mouse position) is inside the element's bounds.
   * Uses the owner's RectTransformComponent for calculations.
   * @param point The world or screen space point to test.
   * @return True if the point is within the rectangle.
   */
  bool CheckCollision(const Vector2Df& point);
};

}  // namespace FalkonEngine