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
void UIProgressBar::SetPivot(Vector2Df pivot) {
  UIObject::SetPivot(pivot);
  m_fillObject->SetPivot(pivot);
}

//------------------------------------------------------------------------------------------------------------
void UIProgressBar::OnNotify(const GameEvent& event) {
  // FILTERING: Ensure the event matches our registered interest
  if (event.type != m_targetEventType) {
    return;
  }

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
  //m_currentValue = std::clamp(percent, 0.0f, 1.0f);
  //UpdateFillVisuals();

  // 1. Нормализуем входной процент (0..1)
  float clampedPercent = std::clamp(percent, 0.0f, 1.0f);

  // 2. Вычисляем рабочую область (чистый диапазон)
  // Например, если боттом 0.2, а топ 0.1, рабочая область = 1.0 - 0.2 - 0.1 = 0.7
  float workRange = 1.0f - m_topOffsetPers - m_bottonOffsetPers;

  // 3. Пересчитываем в эффективный диапазон
  // При 0% мы получаем bottonOffsetPers, при 100% получаем 1.0 - topOffsetPers
  m_currentValue = m_bottonOffsetPers + (clampedPercent * workRange);

  UpdateFillVisuals();
}

//------------------------------------------------------------------------------------------------------------
void UIProgressBar::SetProgressBarConfig(const std::string& bgPath, const std::string& fillPath, sf::Color color,
                                         FillDirection dir, float topOffsetPers, float bottonOffsetPers) {
  m_direction = dir;
  SetTexture(bgPath);

  if (m_fillObject) {
    m_fillObject->SetTexture(fillPath);
    // Sync initial size to match the background dimensions
    m_fillObject->SetSize(GetSize());
    m_fillObject->SetColor(color);
    m_topOffsetPers = topOffsetPers;
    m_bottonOffsetPers = bottonOffsetPers;
  }
  UpdateFillVisuals();
}

//------------------------------------------------------------------------------------------------------------
void UIProgressBar::UpdateFillVisuals() {
  if (!m_fillObject) return;
  auto fillComp = m_fillObject->GetComponent<UIImageComponent>();
  auto transform = m_fillObject->GetComponent<TransformComponent>();

  sf::IntRect fullRect = fillComp->GetFullTextureRect();
  sf::IntRect clippedRect = fullRect;
  Vector2Df pivot;
  float sizeDelta = 0.0f;
  float offsetY = 0.0f;
  float offsetX = 0.0f;

  switch (m_direction) {
    case FillDirection::LeftToRight:
      clippedRect.width = static_cast<int>(fullRect.width * m_currentValue);
      pivot = {1.0f, 0.0f};
      break;
    case FillDirection::RightToLeft:
      clippedRect.width = static_cast<int>(fullRect.width * m_currentValue);
      clippedRect.left = fullRect.width - clippedRect.width;
      pivot = {1.0f, 0.0f};
      sizeDelta = 1 - (static_cast<float>(clippedRect.width) / static_cast<float>(fullRect.width));
      offsetX = GetSize().x * sizeDelta * pivot.x;
      break;
    case FillDirection::BottomToTop:
      clippedRect.height = static_cast<int>(fullRect.height * m_currentValue);
      clippedRect.top = fullRect.height - clippedRect.height;
      pivot = {0.0f, 1.0f};
      sizeDelta = 1 - (static_cast<float>(clippedRect.height) / static_cast<float>(fullRect.height));
      offsetY = GetSize().y * sizeDelta * pivot.y;
      break;
    case FillDirection::TopToBottom:
      clippedRect.height = static_cast<int>(fullRect.height * m_currentValue);
      pivot = {0.0f, 0.0f};
      break;
  }

  fillComp->SetTextureRect(clippedRect);
  m_fillObject->SetPivot(pivot);
  transform->SetLocalPosition(offsetX, offsetY);
}

}  // namespace FalkonEngine