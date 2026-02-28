#include "pch.h"

#include "GameObject.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "TransformComponent.h"
#include "UITextComponent.h"

namespace FalkonEngine {

// UITextComponent --- Low-level SFML text rendering wrapper ---
//-----------------------------------------------------------------------------------------------------------
UITextComponent::UITextComponent(GameObject* gameObject) : Component(gameObject) {
  // INITIALIZATION: Set default font properties
  m_text.setCharacterSize(24);
  m_text.setFillColor(sf::Color::White);
}

//-----------------------------------------------------------------------------------------------------------
void UITextComponent::SetFont(const std::string& name) {
  // RESOURCE BINDING: Retrieve the shared font pointer from the ResourceSystem
  const sf::Font* font = ResourceSystem::Instance()->GetFontShared(name);
  if (font) {
    m_text.setFont(*font);
    p_fontAsset = font;
  }
}

//-----------------------------------------------------------------------------------------------------------
void UITextComponent::SetString(const std::string& content) { m_text.setString(content); }

//-----------------------------------------------------------------------------------------------------------
void UITextComponent::SetCharacterSize(unsigned int size) { m_text.setCharacterSize(size); }

//-----------------------------------------------------------------------------------------------------------
void UITextComponent::SetColor(sf::Color color) { m_text.setFillColor(color); }

//-----------------------------------------------------------------------------------------------------------
void UITextComponent::Render() {
  auto transform = p_gameObject->GetComponent<TransformComponent>();
  if (!transform || !p_fontAsset) return;

  // SYNCHRONIZATION: Map the SFML text position and rotation to the GameObject's world transform
  m_text.setPosition(transform->GetWorldPosition().x, transform->GetWorldPosition().y);
  m_text.setRotation(transform->GetWorldRotation());

  // DRAW CALL: Dispatch the drawable object to the global RenderSystem
  RenderSystem::Instance()->Render(m_text);
}

//-----------------------------------------------------------------------------------------------------------
void UITextComponent::SyncTransform(Vector2Df size, Vector2Df pivot) {
  sf::FloatRect bounds = m_text.getLocalBounds();

  // ALIGNMENT LOGIC: Account for SFML glyph offsets (left/top) to ensure precise pivot-based positioning
  m_text.setOrigin(bounds.left + (bounds.width * pivot.x), bounds.top + (bounds.height * pivot.y));
}

}  // namespace FalkonEngine