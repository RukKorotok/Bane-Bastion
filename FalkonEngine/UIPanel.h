#pragma once

#include "pch.h"
#include "UICommandRegistry.h"
#include "UIObject.h"

namespace FalkonEngine {

/**
 * @brief A structural UI container used for grouping and organizing other UI elements.
 * UIPanel acts as a parent object in the UI hierarchy, allowing for coordinated
 * transformations and visibility management of its children.
 */
class UIPanel : public UIObject {
 public:
  /**
   * @brief Inherits constructors from UIObject.
   * Facilitates standard initialization via GameObject/UIObject patterns.
   */
  using UIObject::UIObject;

 protected:
  /**
   * @brief Internal event handler for panel-specific logic.
   * Currently serves as a pass-through as panels primarily act as containers.
   * @param event The GameEvent to be processed.
   */
  void HandleEvent(const GameEvent& event) override {}
};

}  // namespace FalkonEngine