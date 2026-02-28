#include "pch.h"

#include "UICommandRegistry.h"

namespace FalkonEngine {

// UICommandRegistry
//------------------------------------------------------------------------------------------------------------
void UICommandRegistry::Bind(const std::string& actionID, UIAction action) { m_commands[actionID] = action; }

void UICommandRegistry::Execute(const std::string& actionID) {
  if (actionID.empty() || actionID == "NONE") {
    return;
  }

  auto it = m_commands.find(actionID);
  if (it != m_commands.end()) {
    it->second();  // Вызываем привязанную функцию
  } else {
    // Логируем ошибку, если в JSON написали абракадабру
    std::cout << "[UI Error] Command not found: " << actionID << std::endl;
  }
}

//------------------------------------------------------------------------------------------------------------
void UICommandRegistry::Clear() { m_commands.clear(); }

}  // namespace FalkonEngine