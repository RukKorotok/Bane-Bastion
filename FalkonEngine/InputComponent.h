#pragma once

#include <SFML/Window.hpp>

#include "Component.h"
#include "Vector.h"

namespace FalkonEngine {

/**
 * @brief Component responsible for processing and storing local player input.
 * Polls keyboard and mouse states to provide normalized axis values for
 * movement and aiming logic.
 */
class InputComponent : public Component {
 public:
  /**
   * @brief Constructs an InputComponent for a specific GameObject.
   * @param gameObject Pointer to the owner GameObject.
   */
  InputComponent(GameObject* gameObject);

  /**
   * @brief Polls current input states (Keyboard/Mouse) and updates axis values.
   * @param deltaTime Time elapsed since the last frame.
   */
  void Update(float deltaTime) override;

  /**
   * @brief Not used for this component.
   */
  void Render() override;

  /** * @brief Retrieves the current horizontal axis value.
   * @return Float in range [-1.0, 1.0] (e.g., A/D or Left/Right arrows).
   */
  float GetHorizontalAxis() const;

  /** * @brief Retrieves the current vertical axis value.
   * @return Float in range [-1.0, 1.0] (e.g., W/S or Up/Down arrows).
   */
  float GetVerticalAxis() const;

 private:
  float m_horizontalAxis = 0.f;  ///< Cached horizontal input value.
  float m_verticalAxis = 0.f;    ///< Cached vertical input value.
  Vector2Df m_lastMousePos;      ///< Stores the mouse coordinates from the previous frame.
};

}  // namespace FalkonEngine