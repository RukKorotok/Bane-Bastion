#pragma once

#include "UISelectable.h"

namespace FalkonEngine {

/**
 * @brief Interactive UI element that allows users to select a value by sliding a handle.
 * Inherits from UISelectable to leverage hover and click states, while adding
 * specific logic for dragging and normalized value (0.0 to 1.0) calculation.
 */
class UISlider : public UISelectable {
 public:
  /**
   * @brief Constructs a UISlider with a specific name.
   * @param name Name of the UI object.
   */
  UISlider(std::string name);

  /**
   * @brief Initializes the slider, ensuring all child components and visuals are ready.
   */
  void Awake() override;

  // --- (UISelectable Overrides) ---

  /** @brief Initiates the dragging process if the cursor is within bounds. */
  void OnPointerDown(const GameEvent& e) override;

  /** @brief Ends the dragging process. */
  void OnPointerUp(const GameEvent& e) override;

  /** @brief Updates the slider value and handle position while dragging. */
  void OnPointerMove(const GameEvent& e) override;

  /**
   * @brief Configures the visual appearance and initial state of the slider.
   * @param bgPath Resource path for the background texture.
   * @param handlePath Resource path for the movable handle texture.
   * @param actionID Identifier for the action triggered by value changes.
   * @param startValue Initial normalized value (0.0 to 1.0).
   */
  void SetSliderConfig(const std::string& bgPath, const std::string& handlePath, const std::string& actionID,
                       float startValue);

  /**
   * @return The current normalized slider value, ranging from 0.0 to 1.0.
   */
  float GetValue() const { return m_value; }

 private:
  /**
   * @brief Updates the visual position of the handle based on the current m_value.
   */
  void UpdateHandlePosition();

  /**
   * @brief Translates mouse/pointer coordinates into a normalized slider value.
   * @param mousePos Current position of the pointer in world/UI space.
   */
  void CalculateValue(Vector2Df mousePos);

  class UIImage* m_handleObject = nullptr;  ///< Reference to the visual handle component.

  float m_value = 0.5f;       ///< Current normalized value (0.0 to 1.0).
  bool m_isDragging = false;  ///< Flag indicating if the user is currently sliding the handle.
};

}  // namespace FalkonEngine