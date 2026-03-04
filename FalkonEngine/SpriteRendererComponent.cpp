#include "pch.h"

#include "SpriteRendererComponent.h"

#include "RenderSystem.h"
#include "TransformComponent.h"

namespace FalkonEngine {

// SpriteRendererComponent --- Handling 2D graphics submission and sprite transformations ---
//-----------------------------------------------------------------------------------------------------------
SpriteRendererComponent::SpriteRendererComponent(GameObject* gameObject) : Component(gameObject) {
  m_sprite = new sf::Sprite();

  // COORDINATE STANDARDS: Default scale with Y-axis inversion
  m_scale = {1.0f, -1.0f};

  m_transform = gameObject->GetComponent<TransformComponent>();

  // LIFECYCLE: Ensure the object can be positioned in world space
  FE_CORE_ASSERT(m_transform != nullptr, "SpriteRendererComponent added to GameObject '" + p_gameObject->GetName() +
                                             "' without TransformComponent!");

  FE_APP_TRACE("SpriteRendererComponent initialized for: " + p_gameObject->GetName());
}

//-----------------------------------------------------------------------------------------------------------
SpriteRendererComponent::~SpriteRendererComponent() {
  if (m_sprite != nullptr) {
    delete m_sprite;
    m_sprite = nullptr;
  }
  FE_APP_TRACE("SpriteRendererComponent destroyed for: " + p_gameObject->GetName());
}

//-----------------------------------------------------------------------------------------------------------
void SpriteRendererComponent::Update(float deltaTime) {
  // Logic updates for visual effects could be placed here
}

//-----------------------------------------------------------------------------------------------------------
void SpriteRendererComponent::Render() {
  if (m_sprite != nullptr && m_sprite->getTexture() != nullptr) {
    FE_CORE_ASSERT(m_transform != nullptr, "Transform lost in SpriteRenderer::Render!");

    // SYNCHRONIZATION: Apply world transform data to the SFML sprite
    m_sprite->setPosition(Convert<sf::Vector2f, Vector2Df>(m_transform->GetWorldPosition()));
    m_sprite->setRotation(m_transform->GetWorldRotation());

    Vector2Df transformScale = m_transform->GetWorldScale();

    // TRANSFORMATION: Combine local scale (flipping/sizing) with global world scale
    m_sprite->setScale({m_scale.x * transformScale.x, m_scale.y * transformScale.y});

    // SUBMISSION: Dispatch to the global rendering queue
    RenderSystem::Instance()->Render(*m_sprite);
  } else if (m_sprite != nullptr && m_sprite->getTexture() == nullptr) {
    // ERROR TRACKING: Prevent console spam by only warning when the assignment state changes
    static std::string lastWarnedObject = "";
    if (lastWarnedObject != p_gameObject->GetName()) {
      FE_CORE_WARN("SpriteRenderer on '" + p_gameObject->GetName() + "' has no texture assigned. Skipping render.");
      lastWarnedObject = p_gameObject->GetName();
    }
  }
}

//-----------------------------------------------------------------------------------------------------------
const sf::Sprite* SpriteRendererComponent::GetSprite() const { return m_sprite; }

//-----------------------------------------------------------------------------------------------------------
void SpriteRendererComponent::SetTexture(const sf::Texture& newTexture) {
  if (m_sprite == nullptr) {
    return;
  }
  m_sprite->setTexture(newTexture);

  // AUTO-PIVOT: Calculate origin based on current texture rect or full texture size
  sf::IntRect currentRect = m_sprite->getTextureRect();

  float width = (currentRect.width > 0) ? (float)currentRect.width : (float)newTexture.getSize().x;
  float height = (currentRect.height > 0) ? (float)currentRect.height : (float)newTexture.getSize().y;

  m_sprite->setOrigin({0.5f * width, 0.5f * height});

  FE_CORE_INFO("Texture set for '" + p_gameObject->GetName() + "'. Size: " + std::to_string(width) + "x" +
               std::to_string(height));
}

//-----------------------------------------------------------------------------------------------------------
void SpriteRendererComponent::SetPixelSize(int newWidth, int newHeight) {
  // REQUEST STORAGE: Store target dimensions and trigger recalculation
  m_targetPixelSize = {(float)newWidth, (float)newHeight};
  m_useTargetSize = true;
  ApplyCurrentPixelSize();
}

//-----------------------------------------------------------------------------------------------------------
void SpriteRendererComponent::FlipX(bool flip) {
  if (flip != m_isFlipX) {
    m_scale.x = -m_scale.x;
    m_isFlipX = flip;
  }
}

//-----------------------------------------------------------------------------------------------------------
void SpriteRendererComponent::FlipY(bool flip) {
  if (flip != m_isFlipY) {
    m_scale.y = -m_scale.y;
    m_isFlipY = flip;
  }
}

//-----------------------------------------------------------------------------------------------------------
void SpriteRendererComponent::ApplyCurrentPixelSize() {
  if (!m_useTargetSize || !m_sprite || !m_sprite->getTexture()) {
    FE_CORE_ERROR("ApplyCurrentPixelSize failed: Component state or texture invalid on " + p_gameObject->GetName());
    return;
  }

  sf::IntRect rect = m_sprite->getTextureRect();

  // DYNAMIC SCALING: Use current frame rect or fall back to full texture dimensions
  float frameW = (rect.width > 0) ? (float)rect.width : (float)m_sprite->getTexture()->getSize().x;
  float frameH = (rect.height > 0) ? (float)rect.height : (float)m_sprite->getTexture()->getSize().y;

  if (frameW == 0 || frameH == 0) {
    FE_CORE_ERROR("ApplyCurrentPixelSize: Source dimensions are zero on " + p_gameObject->GetName());
    return;
  }

  // CALCULATION: Ratio between desired pixel size and actual source pixels
  float scaleX = m_targetPixelSize.x / frameW;
  float scaleY = m_targetPixelSize.y / frameH;

  // Apply scale with Y-axis correction and update pivot point
  m_scale = {scaleX, -scaleY};
  m_sprite->setOrigin(frameW * 0.5f, frameH * 0.5f);
}

}  // namespace FalkonEngine