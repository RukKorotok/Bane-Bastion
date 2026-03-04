#include "pch.h"
#include "AnimState.h"

namespace FalkonEngine {

// AnimState --- Logic for individual animation state playback and timing ---
//-----------------------------------------------------------------------------------------------------------
AnimState::AnimState(const Animation& anim, const sf::Texture* texture, bool loop)
    : m_animation(anim), m_texture(texture), m_isLooping(loop) {}

//-----------------------------------------------------------------------------------------------------------
void AnimState::OnEnter() {
  // RESET: Ensure playback starts from the first frame whenever the state is activated
  m_elapsedTime = 0.0f;
  m_currentFrameIndex = 0;
  m_isFinished = false;
}

//-----------------------------------------------------------------------------------------------------------
void AnimState::Update(float deltaTime) {
  // TERMINATION CHECK: Stop processing if the animation is static or has completed playback
  if (m_isFinished || m_animation.GetFrameCount() == 0) {
    return;
  }

  // ACCUMULATION: Advance internal timer modified by the playback speed
  m_elapsedTime += deltaTime * m_speedMultiplier;

  const auto& currentFrame = m_animation.GetFrame(m_currentFrameIndex);

  // FRAME TRANSITION: Check if enough time has passed to move to the next frame
  if (m_elapsedTime >= currentFrame.duration) {
    m_elapsedTime = 0.0f;
    m_currentFrameIndex++;

    // BOUNDARY HANDLING: Manage looping or final frame clamping
    if (m_currentFrameIndex >= m_animation.GetFrameCount()) {
      if (m_isLooping) {
        m_currentFrameIndex = 0;
      } else {
        // CLAMP: Stay on the last frame and mark as completed
        m_currentFrameIndex = m_animation.GetFrameCount() - 1;
        m_isFinished = true;
      }
    }
  }
}

//-----------------------------------------------------------------------------------------------------------
const sf::IntRect& AnimState::GetCurrentRect() const { return m_animation.GetFrame(m_currentFrameIndex).rect; }

//-----------------------------------------------------------------------------------------------------------
const sf::Texture* AnimState::GetTexture() const { return m_texture; }

//-----------------------------------------------------------------------------------------------------------
void AnimState::SetSpeed(float speed) { m_speedMultiplier = speed; }

//-----------------------------------------------------------------------------------------------------------
float AnimState::GetSpeed() const { return m_speedMultiplier; }

//-----------------------------------------------------------------------------------------------------------
bool AnimState::IsFinished() const { return m_isFinished; }

}  // namespace FalkonEngine