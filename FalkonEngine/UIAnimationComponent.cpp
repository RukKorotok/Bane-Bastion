#include "pch.h"

#include "GameEvents.h"
#include "GameObject.h"
#include "UIAnimationComponent.h"

namespace FalkonEngine {

// UIAnimationComponent --- Logic for frame sequencing and playback control ---
//-----------------------------------------------------------------------------------------------------------
UIAnimationComponent::UIAnimationComponent(GameObject* gameObject)
    : Component(gameObject), m_currentFrameIndex(0), m_timer(0.0f), m_speedMultiplier(1.0f) {}

//-----------------------------------------------------------------------------------------------------------
void UIAnimationComponent::AddFrame(int x, int y, int w, int h, float duration) {
  // REGISTRATION: Push a manually defined frame into the sequence
  m_frames.push_back({{x, y, w, h}, duration});
}

//-----------------------------------------------------------------------------------------------------------
void UIAnimationComponent::AddFramesLine(int startX, int startY, int width, int height, int count, float frameTime) {
  // GENERATION: Automatically slice a horizontal row from the texture atlas
  for (int i = 0; i < count; ++i) {
    int currentX = startX + (i * width);
    AddFrame(currentX, startY, width, height, frameTime);
  }
}

//-----------------------------------------------------------------------------------------------------------
void UIAnimationComponent::Update(float deltaTime) {
  if (m_frames.empty() || m_speedMultiplier <= 0.0f) return;

  m_timer += deltaTime;

  // CALCULATION: Apply the speed multiplier to the base frame duration
  // Example: If duration is 0.1s and multiplier is 2.0f, the frame changes in 0.05s
  float effectiveDuration = m_frames[m_currentFrameIndex].duration / m_speedMultiplier;

  if (m_timer >= effectiveDuration) {
    m_timer = 0.0f;

    // ADVANCE: Move to the next frame and loop back if at the end
    m_currentFrameIndex = (m_currentFrameIndex + 1) % m_frames.size();

    // NOTIFICATION: Signal the owner (UIAnimate) to update the visual sprite rect
    GameEvent frameEvent;
    frameEvent.type = FalkonEngine::GameEventType::UpdatedAnimation;
    p_gameObject->OnNotify(frameEvent);
  }
}

//-----------------------------------------------------------------------------------------------------------
const sf::IntRect& UIAnimationComponent::GetCurrentRect() const {
  // ACCESSOR: Return the current rect for rendering synchronization
  if (m_currentFrameIndex < m_frames.size()) {
    return m_frames[m_currentFrameIndex].rect;
  }
  return m_emptyRect;
}

}  // namespace FalkonEngine