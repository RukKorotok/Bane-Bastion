#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "TransformComponent.h"
#include "Vector.h"

namespace FalkonEngine {

/**
 * @brief Component responsible for rendering 2D sprites.
 * Handles texture management, pixel-perfect sizing, and sprite flipping,
 * while synchronizing its position with the GameObject's TransformComponent.
 */
class SpriteRendererComponent : public Component {
 public:
  /**
   * @brief Constructs a SpriteRendererComponent for a specific GameObject.
   * @param gameObject Pointer to the owner GameObject.
   */
  SpriteRendererComponent(GameObject* gameObject);

  /**
   * @brief Destructor. Cleans up the internal SFML sprite resource.
   */
  ~SpriteRendererComponent();

  /**
   * @brief Updates the component's internal state.
   * @param deltaTime Time elapsed since the last frame.
   */
  void Update(float deltaTime) override;

  /**
   * @brief Submits the sprite to the RenderSystem for drawing.
   */
  void Render() override;

  /**
   * @brief Provides read-only access to the internal SFML sprite.
   * @return Constant pointer to the sf::Sprite.
   */
  const sf::Sprite* GetSprite() const;

  /**
   * @brief Assigns a new texture to the sprite.
   * @param newTexture Reference to the SFML texture to be applied.
   */
  void SetTexture(const sf::Texture& newTexture);

  /**
   * @brief Resizes the sprite to specific pixel dimensions.
   * Adjusts the internal scale to match the desired width and height.
   * @param newWidth Targeted width in pixels.
   * @param newHeight Targeted height in pixels.
   */
  void SetPixelSize(int newWidth, int newHeight);

  /**
   * @brief Flips the sprite horizontally.
   * @param flip True to flip, false to reset to default.
   */
  void FlipX(bool flip);

  /**
   * @brief Flips the sprite vertically.
   * @param flip True to flip, false to reset to default.
   */
  void FlipY(bool flip);

  /**
   * @brief Forces recalculation of the sprite's scale based on the target pixel size.
   * Crucial for maintaining consistent dimensions when the sprite's texture rect changes (e.g., during animation).
   */
  void ApplyCurrentPixelSize();

 private:
  Vector2Df m_scale;                ///< Current scale of the sprite.
  sf::Sprite* m_sprite;             ///< Internal SFML sprite object.
  TransformComponent* m_transform;  ///< Cached pointer to the owner's transform.

  sf::Vector2f m_targetPixelSize = {0.0f, 0.0f};  ///< Desired size in world pixels.
  bool m_useTargetSize = false;                   ///< Flag to enable automatic size synchronization.

  bool m_isFlipX = false;  ///< Horizontal flip state.
  bool m_isFlipY = false;  ///< Vertical flip state.
};

}  // namespace FalkonEngine