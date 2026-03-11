#include "pch.h"

#include "SceneManager.h"
#include "UIBoundText.h"

namespace FalkonEngine {

// UIBoundText --- Automated text updates via event binding ---
//-----------------------------------------------------------------------------------------------------------
UIBoundText::UIBoundText(std::string name) : UIText(name) {
  // CONSTRUCTOR: Default initialization
}

//-----------------------------------------------------------------------------------------------------------
void UIBoundText::Awake() {
  // INITIALIZATION: Standard UIText setup
  UIText::Awake();

  // REGISTRATION: Subscribe to the input/event stream (adjust to your Global Event System if different)
  SceneManager::Instance().GetActiveScene()->GetPlayerController()->Subscribe(this);
}

//-----------------------------------------------------------------------------------------------------------
void UIBoundText::ConfigureBinding(GameEventType type, std::string key, std::string prefix, std::string suffix) {
  m_targetEventType = type;
  m_dataKey = key;
  m_prefix = prefix;
  m_suffix = suffix;
}

//-----------------------------------------------------------------------------------------------------------
void UIBoundText::OnNotify(const GameEvent& event) {
  // FILTERING: Check if the event type matches our binding
  if (event.type != m_targetEventType) return;

  // DATA EXTRACTION: Search for the bound key in the event's data map
  auto it = event.input.find(m_dataKey);
  if (it != event.input.end()) {
    // FORMATTING: Construct the final string (e.g., "Score: " + "100" + " pts")
    // Note: Assuming it->second can be converted to string or is a float
    std::string valueStr = std::to_string(static_cast<int>(it->second));

    SetText(m_prefix + valueStr + m_suffix);
  }
}

}  // namespace FalkonEngine