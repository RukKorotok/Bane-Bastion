#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Component.h"

namespace FalkonEngine {

/**
 * @brief Low-level component for rendering text using SFML.
 * Manages sf::Text properties and synchronizes with UI transformation logic.
 */
class UITextComponent : public Component {
 public:
  /**
   * @brief Constructs a UITextComponent.
   * @param gameObject Pointer to the owner GameObject.
   */
  explicit UITextComponent(GameObject* gameObject);

  /**
   * @brief Assigns a font from the ResourceSystem.
   * @param name The registered name of the font asset.
   */
  void SetFont(const std::string& name);

  /** @brief Updates the displayed string content. */
  void SetString(const std::string& content);

  /** @brief Sets the character size in pixels. */
  void SetCharacterSize(unsigned int size);

  /** @brief Sets the fill color of the text. */
  void SetColor(sf::Color color);

  /**
   * @brief Aligns the text origin based on RectTransform dimensions and pivot.
   * @param size The logical size of the UI element.
   * @param pivot The normalized pivot point (0.0 to 1.0).
   */
  void SyncTransform(Vector2Df size, Vector2Df pivot);

  /**
   * @brief Mandatory update override; currently no per-frame logic required.
   */
  void Update(float deltaTime) override {}

  /**
   * @brief Renders the text to the RenderSystem.
   */
  void Render() override;

  /* --- Accessors --- */
  std::string GetString() const { return m_text.getString(); }
  sf::FloatRect GetLocalBounds() const { return m_text.getLocalBounds(); }

 private:
  sf::Text m_text;                        ///< The underlying SFML text object.
  const sf::Font* p_fontAsset = nullptr;  ///< Pointer to the font managed by ResourceSystem.
};

}  // namespace FalkonEngine