#pragma once

#include "UISelectable.h"

namespace FalkonEngine {

/**
 * @brief Standard UI Button component.
 * A specialization of UISelectable that triggers a command execution
 * when the pointer is pressed within its bounds.
 */
class UIButton : public UISelectable {
 public:
  /**
   * @brief Inherits constructors from UISelectable.
   * Uses the standard initialization flow for UI objects.
   */
  using UISelectable::UISelectable;

 protected:
  /**
   * @brief Specialized behavior for button clicks.
   * Overrides the pointer down callback to execute the registered action.
   * @param e The pointer event data.
   */
  void OnPointerDown(const GameEvent& e) override;
};

}  // namespace FalkonEngine