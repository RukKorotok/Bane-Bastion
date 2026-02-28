#pragma once

#include "UISelectable.h"

namespace FalkonEngine {

/**
 * @brief Interactive UI component that toggles between two states: Checked and Unchecked.
 * Inherits from UISelectable to leverage pointer events and adds a secondary
 * visual layer (the "mark") to represent the current state.
 */
class UICheckBox : public UISelectable {
 public:
  /**
   * @brief Inherits constructors from UISelectable.
   */
  using UISelectable::UISelectable;

  /**
   * @brief Initializes the checkbox and prepares the child components.
   */
  void Awake() override;

  /**
   * @brief Overrides the pointer down event to toggle the check state.
   * @param e The pointer event data.
   */
  void OnPointerDown(const GameEvent& e) override;

  /**
   * @brief Configures the textures and commands for the checkbox.
   * @param bgPath Path to the background texture of the checkbox.
   * @param markPath Path to the texture representing the "checked" mark.
   * @param actionOn Command ID to execute when checked.
   * @param actionOff Command ID to execute when unchecked.
   * @param initialState Starting state of the checkbox.
   */
  void SetCheckBoxConfig(const std::string& bgPath, const std::string& markPath, const std::string& actionOn,
                         const std::string& actionOff, bool initialState = false);

 private:
  /**
   * @brief Updates the visibility and visual state of the mark component.
   */
  void UpdateMark();

  bool m_isChecked = false;                     ///< The current toggle state.
  UIImageComponent* m_markComponent = nullptr;  ///< Component used to render the check mark.

  std::string m_actionIDOff;  ///< Specific action ID for the "Off" state.
};
}  // namespace FalkonEngine