#pragma once

#include "UIImageComponent.h"
#include "UIPanel.h"

namespace FalkonEngine {

/**
 * @brief High-level UI object that renders a sprite.
 * Inherits from UIPanel to maintain hierarchy and uses UIImageComponent
 * for rendering. It automatically synchronizes the sprite's dimensions
 * with the object's RectTransform.
 */
class UIImage : public UIPanel {
 public:
  /**
   * @brief Inherits constructors from UIPanel.
   */
  using UIPanel::UIPanel;

  /**
   * @brief Initializes the object and automatically attaches a UIImageComponent.
   */
  void Awake() override;

  /**
   * @brief Sets the texture for the underlying image component.
   * @param path File path to the texture resource.
   */
  void SetTexture(const std::string& path);

  /**
   * @brief Sets the color tint of the image.
   * @param color SFML Color to apply.
   */
  void SetColor(sf::Color color);

  /**
   * @brief Resizes the object and immediately syncs the image scale.
   * @param size New dimensions in pixels.
   */
  void SetSize(Vector2Df size) override;

 protected:
  /** * @brief Cached pointer to the image component to avoid frequent GetComponent calls.
   */
  UIImageComponent* p_imageComponent = nullptr;

 private:
  /**
   * @brief Internal helper to adjust the image component scale based on the current RectTransform size.
   */
  void SyncImageScale();
};

}  // namespace FalkonEngine