#include "pch.h"

#include "SceneManager.h"
#include "UICommandRegistry.h"
#include "UIImageComponent.h"
#include "UISlider.h"

namespace FalkonEngine {

// UISlider --- Interactive slider logic for range-based input ---
//------------------------------------------------------------------------------------------------------------
UISlider::UISlider(std::string name) : UISelectable(name) {}

//------------------------------------------------------------------------------------------------------------
void UISlider::Awake() {
  UISelectable::Awake();

  // HIERARCHY SETUP: Create the handle as a child GameObject within the current scene world
  auto world = FalkonEngine::SceneManager::Instance().GetActiveScene()->GetWorld();
  m_handleObject = world->CreateGameObject<UIImage>("SliderHandle");
  m_handleObject->Awake();

  // Establish parent-child relationship for relative transformation
  AddChild(m_handleObject);
}

//------------------------------------------------------------------------------------------------------------
void UISlider::OnPointerDown(const GameEvent& event) {
  m_isDragging = true;
  CalculateValue(event.direction);
}

//------------------------------------------------------------------------------------------------------------
void UISlider::OnPointerUp(const GameEvent& event) { m_isDragging = false; }

//------------------------------------------------------------------------------------------------------------
void UISlider::OnPointerMove(const GameEvent& event) {
  if (m_isDragging) {
    CalculateValue(event.direction);
  }
}

//------------------------------------------------------------------------------------------------------------
void UISlider::CalculateValue(Vector2Df mousePos) {
  auto transform = GetComponent<TransformComponent>();
  if (!transform) {
    return;
  }

  Vector2Df pos = transform->GetWorldPosition();
  Vector2Df size = GetSize();
  Vector2Df pivot = GetPivot();

  // MATH: Determine the start (left edge) of the slider in world space
  float leftWorldEdge = pos.x - (size.x * pivot.x);

  // NORMALIZATION: Map the mouse X position to a 0.0 - 1.0 range
  float localX = mousePos.x - leftWorldEdge;
  float newValue = std::clamp(localX / size.x, 0.0f, 1.0f);

  if (m_value != newValue) {
    m_value = newValue;
    UpdateHandlePosition();

    // SIGNALING: Execute the associated command (e.g., sound volume or brightness)
    UICommandRegistry::Execute(p_actionID);
  }
}

//------------------------------------------------------------------------------------------------------------
void UISlider::UpdateHandlePosition() {
  if (!m_handleObject) return;

  auto handleTransform = m_handleObject->GetComponent<TransformComponent>();
  if (handleTransform) {
    Vector2Df sliderSize = GetSize();
    Vector2Df pivot = GetPivot();

    // LOCAL COORDINATES: Calculate the handle's X based on normalized m_value
    float localLeftEdge = -sliderSize.x * pivot.x;
    float xPos = localLeftEdge + (sliderSize.x * m_value);

    // VERTICAL CENTERING: Align the handle based on the slider's pivot
    float yPos = (0.5f - pivot.y) * sliderSize.y;

    handleTransform->SetLocalPosition({xPos, yPos});
  }
}

//------------------------------------------------------------------------------------------------------------
void UISlider::SetSliderConfig(const std::string& bgPath, const std::string& handlePath, const std::string& actionID,
                               float startValue) {
  p_actionID = actionID;
  m_value = startValue;

  // Set the background visual
  SetTexture(bgPath);

  if (m_handleObject) {
    // Set the handle visual and adjust its size to be slightly larger than the track
    m_handleObject->SetTexture(handlePath);
    m_handleObject->SetSize({100.0f, GetSize().y + 50.0f});
  }

  UpdateHandlePosition();
}

}  // namespace FalkonEngine