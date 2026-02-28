#include "pch.h"

#include "SpriteRendererComponent.h"

#include "RenderSystem.h"
#include "TransformComponent.h"

namespace FalkonEngine {

// SpriteRendererComponent --- Handling 2D graphics submission and sprite transformations ---
//-----------------------------------------------------------------------------------------------------------
SpriteRendererComponent::SpriteRendererComponent(GameObject* gameObject) : Component(gameObject) {
  m_sprite = new sf::Sprite();

  // Default scale with Y-axis inversion to match engine coordinate standards
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

    // Combine component-specific scale (flipping/sizing) with global transform scale
    m_sprite->setScale({m_scale.x * transformScale.x, m_scale.y * transformScale.y});

    // Submission to the global rendering queue
    RenderSystem::Instance()->Render(*m_sprite);
  } else if (m_sprite != nullptr && m_sprite->getTexture() == nullptr) {
    // ERROR TRACKING: Prevent console spam by only warning when the object changes
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

  // AUTO-PIVOT: Center the origin based on new texture dimensions
  sf::Vector2u textureSize = newTexture.getSize();
  m_sprite->setOrigin({0.5f * textureSize.x, 0.5f * textureSize.y});

  FE_CORE_INFO("Texture set for '" + p_gameObject->GetName() + "'. Size: " + std::to_string(textureSize.x) + "x" +
               std::to_string(textureSize.y));
}

//-----------------------------------------------------------------------------------------------------------
void SpriteRendererComponent::SetPixelSize(int newWidth, int newHeight) {
  if (m_sprite == nullptr || m_sprite->getTexture() == nullptr) {
    FE_CORE_ERROR("Cannot SetPixelSize: Sprite or Texture is null on " + p_gameObject->GetName());
    return;
  }

  auto originalSize = m_sprite->getTexture()->getSize();

  if (originalSize.x == 0 || originalSize.y == 0) {
    FE_CORE_ERROR("SetPixelSize failed: Texture size is zero on " + p_gameObject->GetName());
    return;
  }

  // SCALING: Calculate ratio between original texture and desired pixel dimensions
  m_scale = {(float)newWidth / (float)originalSize.x, -(float)newHeight / (float)originalSize.y};
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

}  // namespace FalkonEngine