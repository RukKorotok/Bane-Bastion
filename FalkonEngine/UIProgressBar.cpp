#include "pch.h"

#include <algorithm>
#include "SceneManager.h"
#include "TransformComponent.h"
#include "UIImageComponent.h"
#include "UIProgressBar.h"

namespace FalkonEngine {

// UIProgressBar --- Visual representation of a status bar with dynamic clipping ---
//------------------------------------------------------------------------------------------------------------
UIProgressBar::UIProgressBar(std::string name) : UIImage(name) {}

//------------------------------------------------------------------------------------------------------------
void UIProgressBar::Awake() {
  // 1. Initialize the background (via base UIImage)
  UIImage::Awake();

  // 2. CHILD HIERARCHY: Create the fill layer as a distinct GameObject.
  // This allows the fill to have its own Transform and Rendering components.
  auto scene = SceneManager::Instance().GetActiveScene();
  if (scene && scene->GetWorld()) {
    m_fillObject = scene->GetWorld()->CreateGameObject<UIImage>(GetName() + "_Fill");
    m_fillObject->Awake();

    // Establish parent-child relationship so the fill moves with the background
    AddChild(m_fillObject);
  }
}

//------------------------------------------------------------------------------------------------------------
void UIProgressBar::ConfigureBinding(GameEventType type, int actionID, std::string valKey, std::string maxKey) {
  m_targetEventType = type;
  m_targetActionID = actionID;
  m_valueKey = valKey;
  m_maxKey = maxKey;
}

//------------------------------------------------------------------------------------------------------------
void UIProgressBar::OnNotify(const GameEvent& event) {
  // FILTERING: Ensure the event matches our registered interest
  if (event.type != m_targetEventType) return;

  // Optional filtering by specific ID (e.g., specific Player ID or Stat ID)
  if (m_targetActionID != -1 && event.actionID != m_targetActionID) return;

  // DATA EXTRACTION: Look for current and max values in the event data map
  auto itVal = event.input.find(m_valueKey);
  auto itMax = event.input.find(m_maxKey);

  if (itVal != event.input.end() && itMax != event.input.end()) {
    float current = itVal->second;
    float max = itMax->second;

    if (max > 0.0f) {
      SetProgress(current / max);
    }
  }
}

//------------------------------------------------------------------------------------------------------------
void UIProgressBar::SetProgress(float percent) {
  m_currentValue = std::clamp(percent, 0.0f, 1.0f);
  UpdateFillVisuals();
}

//------------------------------------------------------------------------------------------------------------
void UIProgressBar::SetProgressBarConfig(const std::string& bgPath, const std::string& fillPath, FillDirection dir) {
  m_direction = dir;
  SetTexture(bgPath);

  if (m_fillObject) {
    m_fillObject->SetTexture(fillPath);
    // Sync initial size to match the background dimensions
    m_fillObject->SetSize(GetSize());
  }
  UpdateFillVisuals();
}

//------------------------------------------------------------------------------------------------------------
void UIProgressBar::UpdateFillVisuals() {
  if (!m_fillObject) return;

  auto fillComp = m_fillObject->GetComponent<UIImageComponent>();
  auto fillTrans = m_fillObject->GetComponent<TransformComponent>();

  // Safeguard: Ensure components are valid
  if (!fillComp || !fillTrans) return;

  sf::IntRect fullRect = fillComp->GetFullTextureRect();
  sf::IntRect clippedRect = fullRect;
  Vector2Df size = GetSize();
  Vector2Df offset = {0.0f, 0.0f};

  // CLIPPING LOGIC: Calculate texture rectangle and position offset.
  // We adjust the offset to compensate for the Pivot (assumed 0.5, 0.5)
  // to keep the filling "anchored" to the correct edge.
  switch (m_direction) {
    case FillDirection::LeftToRight:
      clippedRect.width = static_cast<int>(fullRect.width * m_currentValue);
      offset.x = -(size.x * 0.5f) * (1.0f - m_currentValue);
      break;

    case FillDirection::RightToLeft:
      clippedRect.width = static_cast<int>(fullRect.width * m_currentValue);
      clippedRect.left = fullRect.width - clippedRect.width;
      offset.x = (size.x * 0.5f) * (1.0f - m_currentValue);
      break;

    case FillDirection::BottomToTop:
      clippedRect.height = static_cast<int>(fullRect.height * m_currentValue);
      clippedRect.top = fullRect.height - clippedRect.height;
      offset.y = (size.y * 0.5f) * (1.0f - m_currentValue);
      break;

    case FillDirection::TopToBottom:
      clippedRect.height = static_cast<int>(fullRect.height * m_currentValue);
      offset.y = -(size.y * 0.5f) * (1.0f - m_currentValue);
      break;
  }

  fillComp->SetTextureRect(clippedRect);
  fillTrans->SetLocalPosition(offset);
}

}  // namespace FalkonEngine