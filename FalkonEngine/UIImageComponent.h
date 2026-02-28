#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Component.h"

namespace FalkonEngine {

/**
 * @brief Component responsible for rendering 2D textures in the UI layer.
 * Manages an SFML Sprite and provides utilities for color tinting, texture
 * rect manipulation, and auto-scaling to match UI dimensions.
 */
class UIImageComponent : public Component {
 public:
  /**
   * @brief Constructs a UIImageComponent for a specific GameObject.
   * @param gameObject Pointer to the owner GameObject.
   */
  UIImageComponent(GameObject* gameObject);

  /**
   * @brief Default destructor.
   */
  virtual ~UIImageComponent() = default;

  /**
   * @brief Loads and sets the texture from a file path.
   * @param texturePath Path to the image resource.
   */
  void SetTexture(const std::string& texturePath);

  /**
   * @brief Sets the color tint of the image.
   * @param color SFML Color (multiplied with the texture).
   */
  void SetColor(sf::Color color);

  /**
   * @return Current color tint of the image.
   */
  sf::Color GetColor() const;

  /**
   * @return A copy of the internal SFML Sprite.
   */
  sf::Sprite GetSprite() const;

  /**
   * @brief Adjusts the sprite scale and origin to fit a specific size and pivot.
   * Often used to match the RectTransform bounds.
   * @param targetSize Desired dimensions in pixels.
   * @param pivot Normalized pivot point (0.0 to 1.0).
   */
  void SetScaleToSize(Vector2Df targetSize, Vector2Df pivot);

  /**
   * @return The full rectangle representing the texture dimensions.
   */
  sf::IntRect GetFullTextureRect() const;

  /**
   * @brief Sets a sub-rectangle of the texture to be displayed (Atlas/Sprite sheet support).
   * @param rect The source rectangle within the texture.
   */
  void SetTextureRect(const sf::IntRect& rect);

  /**
   * @brief Structural component; logic updates are generally not required for static images.
   */
  void Update(float deltaTime) override {}

  /**
   * @brief Renders the sprite to the active window using the object's transform.
   */
  void Render() override;

 private:
  sf::Sprite m_sprite;                   ///< Internal SFML sprite for rendering.
  sf::Color m_color = sf::Color::White;  ///< Current sprite color tint.
};

}  // namespace FalkonEngine