#include "pch.h"

#include "UISelectable.h"

namespace FalkonEngine {

// UISelectable --- Mediator between visual state and user interaction ---
//-----------------------------------------------------------------------------------------------------------
UISelectable::UISelectable(std::string name) : UIImage(name) {}

//-----------------------------------------------------------------------------------------------------------
void UISelectable::Awake() {
  // 1. Initialize visual components (creates UIImageComponent)
  UIImage::Awake();

  // 2. COMPONENT BINDING: Attach the interaction sensor to this object.
  // This allows the object to detect mouse hover, clicks, and movement within its bounds.
  m_interactable = AddComponent<UIInteractable>();
}

//-----------------------------------------------------------------------------------------------------------
void UISelectable::HandleEvent(const GameEvent& event) {
  // EVENT ROUTING: Convert generic UI GameEvents into specific pointer callbacks.
  // This abstraction allows derived classes like buttons or sliders to focus
  // only on logic (e.g., OnPointerDown) rather than event parsing.
  switch (event.type) {
    case GameEventType::PointerDown:
      OnPointerDown(event);
      break;
    case GameEventType::PointerUp:
      OnPointerUp(event);
      break;
    case GameEventType::PointerEnter:
      OnPointerEnter(event);
      break;
    case GameEventType::PointerExit:
      OnPointerExit(event);
      break;
    case GameEventType::PointerMove:
      OnPointerMove(event);
      break;
    default:
      // Other events are ignored by the interactive logic
      break;
  }
}

//-----------------------------------------------------------------------------------------------------------
void UISelectable::SetActionID(const std::string& id) { p_actionID = id; }

}  // namespace FalkonEngine