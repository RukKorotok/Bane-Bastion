#include "pch.h"
#include "UIInteractable.h"
#include "SceneManager.h"
#include "TransformComponent.h"
#include "UIImage.h"

namespace FalkonEngine {

// UIInteractable --- Handles abstract UI interaction states and event routing ---
//-----------------------------------------------------------------------------------------------------------
UIInteractable::UIInteractable(GameObject* gameObject) : Component(gameObject) {
  // REGISTRATION: Subscribing this component to the PlayerController's input event stream
  SceneManager::Instance().GetActiveScene()->GetPlayerController()->Subscribe(this);
}

//-----------------------------------------------------------------------------------------------------------
void UIInteractable::OnNotify(const GameEvent& event) {
  // PREVENT SELF-RECURSION: Ignore events sent by this component
  if (event.sender == this) {
    return;
  }

  // UI CONSUMPTION FILTER: If the input wasn't consumed by the UI system and we aren't
  // already hovering, ignore it (the mouse is likely interacting with the game world)
  if (!event.isConsumedByUI && !m_isHovered && event.type != GameEventType::ActionReleased) {
    return;
  }

  // Prepare a response event to notify other components on this GameObject
  GameEvent responseEvent;
  responseEvent.sender = this;
  responseEvent.direction = event.direction;
  responseEvent.isConsumedByUI = true;

  // --- HOVER LOGIC (Detection of PointerEnter, PointerExit, and PointerMove) ---
  if (event.type == GameEventType::MouseMoved) {
    bool currentlyInside = CheckCollision(event.direction);

    if (currentlyInside && !m_isHovered) {
      m_isHovered = true;
      responseEvent.type = GameEventType::PointerEnter;
      Notify(responseEvent);
    } else if (!currentlyInside && m_isHovered) {
      m_isHovered = false;
      m_isPressed = false;  // Reset press state if the mouse leaves the area
      responseEvent.type = GameEventType::PointerExit;
      Notify(responseEvent);
    }

    if (m_isHovered) {
      responseEvent.type = GameEventType::PointerMove;
      responseEvent.direction = event.direction;
      Notify(responseEvent);
    }
  }

  // --- PRESS LOGIC (PointerDown) ---
  if (event.type == GameEventType::ActionTriggered) {
    if (m_isHovered) {
      m_isPressed = true;
      responseEvent.type = GameEventType::PointerDown;
      Notify(responseEvent);
    }
  }

  // --- RELEASE LOGIC (PointerUp) ---
  if (event.type == GameEventType::ActionReleased) {
    if (m_isPressed) {
      m_isPressed = false;
      responseEvent.type = GameEventType::PointerUp;
      Notify(responseEvent);

      // LOGIC NOTE: A "Click" event can be explicitly sent here if
      // the button is released while still hovering over the object.
    }
  }
}

//-----------------------------------------------------------------------------------------------------------
bool UIInteractable::CheckCollision(const Vector2Df& point) {
  if (p_gameObject) {
    // HIT DETECTION: Delegate point-in-rect check to the UIObject/RectTransform logic
    auto uiObject = dynamic_cast<UIObject*>(p_gameObject);
    if (uiObject) {
      return uiObject->IsPointInside(point);
    }
  }
  return false;
}

}  // namespace FalkonEngine