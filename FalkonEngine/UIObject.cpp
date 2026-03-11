#include "pch.h"

#include "TransformComponent.h"
#include "UIImageComponent.h"
#include "UIManager.h"
#include "UIObject.h"
#include "PlayerController.h"

namespace FalkonEngine {

//-----------------------------------------------------------------------------------------------------------
UIObject::UIObject() : GameObject("UIObject") {
  // Standard setup for UI layer entities
  SetLayer(RenderLayer::UI);
  m_rectTransform = AddComponent<RectTransformComponent>();
}

//-----------------------------------------------------------------------------------------------------------
UIObject::UIObject(std::string name) : GameObject(name) {
  SetLayer(RenderLayer::UI);
  m_rectTransform = AddComponent<RectTransformComponent>();
}

//-----------------------------------------------------------------------------------------------------------
void UIObject::Awake() {
  // Ensure the layer is consistently set during the initialization lifecycle
  SetLayer(RenderLayer::UI);
}

//-----------------------------------------------------------------------------------------------------------
void UIObject::Update(float deltaTime) {
  // 1. Update all attached components normally (logic, animations, etc.)
  GameObject::Update(deltaTime);

  // 2. Synchronize visual and spatial components
  auto rect = GetComponent<RectTransformComponent>();
  auto image = GetComponent<UIImageComponent>();

  if (rect && image) {
    // UIObject acts as a mediator: it passes sizing data from the transform to the visual sprite
    image->SetScaleToSize(rect->GetSize(), rect->GetPivot());
  }
}

//-----------------------------------------------------------------------------------------------------------
bool UIObject::IsPointInside(Vector2Df point) const {
  if (!m_rectTransform) return false;

  auto transform = GetComponent<TransformComponent>();
  if (!transform) return false;

  Vector2Df pos = transform->GetWorldPosition();
  Vector2Df size = m_rectTransform->GetSize();
  Vector2Df pivot = m_rectTransform->GetPivot();

  // CALCULATE BOUNDS (AABB): Determine the actual screen space coordinates based on the pivot
  float left = pos.x - (size.x * pivot.x);
  float top = pos.y - (size.y * pivot.y);
  float right = left + size.x;
  float bottom = top + size.y;

  return (point.x >= left && point.x <= right && point.y >= top && point.y <= bottom);
}

//-----------------------------------------------------------------------------------------------------------
void UIObject::SetSize(Vector2Df size) {
  if (m_rectTransform) {
    m_rectTransform->SetSize(size);

    // Update the UIManager's hitbox registry for efficient click detection
    auto transform = GetComponent<TransformComponent>();
    if (transform) {
      // For HUD/UI elements, LocalPosition is typically equivalent to ScreenSpace coordinates
      Vector2Df pos = transform->GetLocalPosition();
      float left = pos.x - (m_rectTransform->GetSize().x * GetPivot().x);
      float top = pos.y - (m_rectTransform->GetSize().y * GetPivot().y);

      sf::FloatRect bounds(left, top, m_rectTransform->GetSize().x, m_rectTransform->GetSize().y);

      UIManager::Instance()->UpdateHitbox(this->GetID(), bounds);
    }
  }
}

//-----------------------------------------------------------------------------------------------------------
Vector2Df UIObject::GetSize() const { return m_rectTransform ? m_rectTransform->GetSize() : Vector2Df(0.f, 0.f); }

//-----------------------------------------------------------------------------------------------------------
void UIObject::SetPivot(Vector2Df pivot) {
  if (m_rectTransform) {
    m_rectTransform->SetPivot(pivot);
  }
}

//-----------------------------------------------------------------------------------------------------------
Vector2Df UIObject::GetPivot() const { return m_rectTransform ? m_rectTransform->GetPivot() : Vector2Df(0.5f, 0.5f); }

//-----------------------------------------------------------------------------------------------------------
void UIObject::HandleEvent(const GameEvent& event) {
  // Specific UI logic for handling system-wide events can be implemented here
}

}  // namespace FalkonEngine