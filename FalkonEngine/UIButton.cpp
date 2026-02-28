#include "pch.h"

#include "UIButton.h"
#include "UICommandRegistry.h"

namespace FalkonEngine {

// UIButton --- Simple command-triggering interface element ---
//------------------------------------------------------------------------------------------------------------
void UIButton::OnPointerDown(const GameEvent& e) {
  // COMMAND EXECUTION: When the button is pressed, look up and run the
  // logic associated with p_actionID in the global registry.
  UICommandRegistry::Execute(p_actionID);
}

}  // namespace FalkonEngine