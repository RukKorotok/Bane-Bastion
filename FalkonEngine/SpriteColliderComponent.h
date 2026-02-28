#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "ColliderComponent.h"
#include "RenderSystem.h"
#include "SpriteRendererComponent.h"

namespace FalkonEngine {

/**
 * @brief Collision component that synchronizes its bounds with a SpriteRenderer.
 * Automatically tracks the sprite's texture dimensions and transformations
 * to provide accurate axis-aligned bounding boxes (AABB).
 */
class SpriteColliderComponent : public ColliderComponent {
 public:
  /**
   * @brief Constructs a SpriteColliderComponent for a specific GameObject.
   * @param gameObject Pointer to the owner GameObject.
   */
  SpriteColliderComponent(GameObject* gameObject);

  /**
   * @brief Default destructor.
   */
  ~SpriteColliderComponent();

  /**
   * @brief Retrieves the current global bounding box of the associated sprite.
   * @return Reference to the SFML FloatRect representing world-space bounds.
   */
  const sf::FloatRect& GetBounds() const;

  /** * @brief Calculates the object's dimensions relative to the grid units.
   * Useful for pathfinding and grid-based positioning.
   * @param ppu Pixels Per Unit (conversion factor).
   * @return A vector representing the size in game world units.
   */
  Vector2Df GetSizeInUnits(float ppu) const;

  /**
   * @brief Synchronizes the collider's state with the owner's transform and sprite.
   * @param deltaTime Time elapsed since the last frame.
   */
  void Update(float deltaTime) override;

  /**
   * @brief Optional debug rendering for the collider bounds.
   */
  void Render() override;

 private:
  const sf::Sprite* m_sprite;  ///< Cached reference to the owner's SpriteRenderer internal sprite.
};

}  // namespace FalkonEngine