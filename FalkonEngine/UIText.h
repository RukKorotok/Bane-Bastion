#pragma once

#include "UIPanel.h"
#include "UITextComponent.h"

namespace FalkonEngine {

/**
 * @brief High-level UI element for rendering and managing text.
 * Integrates with RectTransform logic and provides a simplified API for
 * font and content management.
 */
class UIText : public UIPanel {
 public:
  /**
   * @brief Constructs a UIText object.
   * @param name Unique identifier for the GameObject.
   */
  UIText(std::string name);

  /**
   * @brief Initializes the UI component and attaches the rendering component.
   */
  void Awake() override;

  /* --- User API --- */

  /** @brief Updates the displayed string and refreshes layout bounds. */
  void SetText(const std::string& text);

  /** @brief Sets the font by its registered name in the ResourceSystem. */
  void SetFont(const std::string& path);

  /** @brief Sets the character size in pixels. */
  void SetFontSize(unsigned int size);

  /** @brief Sets the text fill color. */
  void SetColor(sf::Color color);

  /* --- RectTransform Overrides --- */

  /** @brief Updates logical size and synchronizes text origin. */
  void SetSize(Vector2Df size) override;

  /** @brief Updates pivot point and synchronizes text origin. */
  void SetPivot(Vector2Df pivot) override;

 private:
  /** @brief Recalculates the text origin based on current bounds and pivot. */
  void RefreshTextLayout();

  UITextComponent* p_textComponent = nullptr;  ///< Pointer to the underlying renderer.
};

}  // namespace FalkonEngine