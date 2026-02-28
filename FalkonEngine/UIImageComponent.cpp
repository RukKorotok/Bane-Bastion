#include "pch.h"
#include "UIImageComponent.h"
#include "GameObject.h"

#include "RectTransformComponent.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "TransformComponent.h"

namespace FalkonEngine {

// UIImageComponent --- Handles texture rendering and sprite manipulation ---
//-----------------------------------------------------------------------------------------------------------
UIImageComponent::UIImageComponent(GameObject* gameObject) : Component(gameObject) {}

//-----------------------------------------------------------------------------------------------------------
void UIImageComponent::SetTexture(const std::string& texturePath) {
  // RESOURCE MANAGEMENT: Fetch texture from the centralized resource system
  auto texture = ResourceSystem::Instance()->GetTextureShared(texturePath);
  if (texture) {
    m_sprite.setTexture(*texture);
  }
}

//-----------------------------------------------------------------------------------------------------------
void UIImageComponent::SetColor(sf::Color color) {
  m_color = color;
  m_sprite.setColor(m_color);
}

//-----------------------------------------------------------------------------------------------------------
sf::Color UIImageComponent::GetColor() const { return m_sprite.getColor(); }

//-----------------------------------------------------------------------------------------------------------
sf::Sprite UIImageComponent::GetSprite() const { return m_sprite; }

//-----------------------------------------------------------------------------------------------------------
void UIImageComponent::Render() {
  auto transform = p_gameObject->GetComponent<TransformComponent>();
  if (!transform) return;

  // SYNCHRONIZATION: Map the sprite's position and rotation to the GameObject's world transform.
  // In UI contexts, this usually represents screen-space coordinates.
  m_sprite.setPosition(transform->GetWorldPosition().x, transform->GetWorldPosition().y);
  m_sprite.setRotation(transform->GetWorldRotation());

  // DRAW CALL: Submit the sprite to the RenderSystem
  RenderSystem::Instance()->Render(m_sprite);
}

//-----------------------------------------------------------------------------------------------------------
void UIImageComponent::SetScaleToSize(Vector2Df targetSize, Vector2Df pivot) {
  if (m_sprite.getTexture() == nullptr) return;

  sf::FloatRect localBounds = m_sprite.getLocalBounds();

  // CALCULATION: Scale the sprite to match the desired pixel dimensions
  m_sprite.setScale(targetSize.x / localBounds.width, targetSize.y / localBounds.height);

  // PIVOT ALIGNMENT: Set the origin based on the provided normalized pivot (e.g., 0.5 for center)
  m_sprite.setOrigin(localBounds.width * pivot.x, localBounds.height * pivot.y);
}

//-----------------------------------------------------------------------------------------------------------
sf::IntRect UIImageComponent::GetFullTextureRect() const {
  const sf::Texture* tex = m_sprite.getTexture();
  if (tex) {
    // Helper to retrieve the entire texture area as a bounding rectangle
    return sf::IntRect(0, 0, tex->getSize().x, tex->getSize().y);
  }
  return sf::IntRect(0, 0, 0, 0);
}

//-----------------------------------------------------------------------------------------------------------
void UIImageComponent::SetTextureRect(const sf::IntRect& rect) { m_sprite.setTextureRect(rect); }

}  // namespace FalkonEngine