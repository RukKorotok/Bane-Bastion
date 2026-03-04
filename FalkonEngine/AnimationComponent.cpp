#include "pch.h"

#include "AnimationComponent.h"
#include "GameObject.h"

namespace FalkonEngine {

// AnimationComponent --- State-based sprite animation controller ---
//-----------------------------------------------------------------------------------------------------------
AnimationComponent::AnimationComponent(GameObject* gameObject) : Component(gameObject) {
  // DEPENDENCY: Animation requires a renderer to display the frames
  m_renderer = p_gameObject->GetComponent<SpriteRendererComponent>();

  FE_CORE_ASSERT(m_renderer != nullptr,
                 "AnimationComponent on '" + p_gameObject->GetName() + "' requires a SpriteRendererComponent!");

  FE_APP_TRACE("AnimationComponent initialized for: " + p_gameObject->GetName());
}

//-----------------------------------------------------------------------------------------------------------
AnimationComponent::~AnimationComponent() { m_states.clear(); }

//-----------------------------------------------------------------------------------------------------------
void AnimationComponent::AddState(const std::string& name, std::shared_ptr<AnimState> state) { m_states[name] = state; }

//-----------------------------------------------------------------------------------------------------------
void AnimationComponent::SetState(const std::string& name) {
  if (m_currentStateName == name) {
    return;
  }

  auto it = m_states.find(name);
  if (it != m_states.end()) {
    m_currentStateName = name;
    m_currentState = it->second;

    // NOTIFICATION: Trigger entry logic for the new state (e.g., resetting frame index)
    m_currentState->OnEnter();

    FE_APP_TRACE("Animation state changed to: " + name);
  }
}

//-----------------------------------------------------------------------------------------------------------
void AnimationComponent::Update(float deltaTime) {
  if (!m_currentState || !m_renderer) {
    return;
  }

  // ADVANCEMENT: Progress the animation timer and frame calculation
  m_currentState->Update(deltaTime);

  // SYNCHRONIZATION: Apply current state data to the underlying SFML sprite
  auto* sprite = const_cast<sf::Sprite*>(m_renderer->GetSprite());
  if (sprite) {
    // 1. Texture validation: Ensure the renderer uses the atlas required by this state
    const sf::Texture* stateTexture = m_currentState->GetTexture();
    if (stateTexture != nullptr && sprite->getTexture() != stateTexture) {
      m_renderer->SetTexture(*stateTexture);
    }

    // 2. Viewport update: Cut the specific frame from the atlas
    sprite->setTextureRect(m_currentState->GetCurrentRect());

    // 3. Scale correction: Force renderer to adjust its scale to the new frame dimensions
    m_renderer->ApplyCurrentPixelSize();
  }
}

//-----------------------------------------------------------------------------------------------------------
const std::string& AnimationComponent::GetCurrentStateName() const { return m_currentStateName; }

//-----------------------------------------------------------------------------------------------------------
void AnimationComponent::SetCurrentSpeed(float speed) {
  if (m_currentState) {
    m_currentState->SetSpeed(speed);
  }
}

//-----------------------------------------------------------------------------------------------------------
void AnimationComponent::SetStateSpeed(const std::string& name, float speed) {
  auto it = m_states.find(name);
  if (it != m_states.end()) {
    it->second->SetSpeed(speed);
  }
}

}  // namespace FalkonEngine