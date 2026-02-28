#include "pch.h"

#include "GameObject.h"
#include "RectTransformComponent.h"
#include "TransformComponent.h"

namespace FalkonEngine {

// RectTransformComponent --- Structural UI layout and hit detection logic ---
//-----------------------------------------------------------------------------------------------------------
RectTransformComponent::RectTransformComponent(GameObject* gameObject)
    : Component(gameObject), m_size(100.0f, 100.0f), m_pivot(0.5f, 0.5f) {
  // LAYER MANAGEMENT: Automatically move the object to the UI rendering layer
  if (p_gameObject) {
    p_gameObject->SetLayer(RenderLayer::UI);
  }
}

//-----------------------------------------------------------------------------------------------------------
void RectTransformComponent::SetSize(Vector2Df size) { m_size = size; }

//-----------------------------------------------------------------------------------------------------------
Vector2Df RectTransformComponent::GetSize() const { return m_size; }

//-----------------------------------------------------------------------------------------------------------
void RectTransformComponent::SetPivot(Vector2Df pivot) { m_pivot = pivot; }

//-----------------------------------------------------------------------------------------------------------
Vector2Df RectTransformComponent::GetPivot() const { return m_pivot; }

//-----------------------------------------------------------------------------------------------------------
bool RectTransformComponent::Contains(Vector2Df point) const {
  auto transform = p_gameObject->GetComponent<TransformComponent>();
  if (!transform) return false;

  Vector2Df pos = transform->GetWorldPosition();

  // BOUNDARY CALCULATION: Determine the rectangle edges based on the Pivot point
  // Example: If pivot is (0.5, 0.5), the top-left corner is calculated as: pos - (size * 0.5)
  float left = pos.x - (m_size.x * m_pivot.x);
  float top = pos.y - (m_size.y * m_pivot.y);
  float right = left + m_size.x;
  float bottom = top + m_size.y;

  // AABB Check for point inclusion
  return (point.x >= left && point.x <= right && point.y >= top && point.y <= bottom);
}

}  // namespace FalkonEngine