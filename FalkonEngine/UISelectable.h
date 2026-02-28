#pragma once
#include "UIImage.h"
#include "UIInteractable.h"

namespace FalkonEngine {

/**
 * @brief Base class for interactive UI elements that change visual state.
 * Combines UIImage for rendering and UIInteractable for input detection.
 * Provides virtual callbacks for pointer events to simplify the creation
 * of buttons, toggles, and other selectable components.
 */
class UISelectable : public UIImage {
 public:
  /**
   * @brief Constructs a UISelectable object.
   * @param name The unique name for this UI element.
   */
  UISelectable(std::string name);

  /**
   * @brief Initializes the component and attaches the UIInteractable logic.
   */
  void Awake() override;

  /**
   * @brief Centralized event dispatcher that routes GameEvents to specific pointer callbacks.
   * @param event The incoming GameEvent from the messaging system.
   */
  void HandleEvent(const GameEvent& event) override;

 protected:
  // --- (Pointer Callbacks: To be overridden by derived classes) ---

  /** @brief Triggered when the pointer/mouse button is pressed down on the element. */
  virtual void OnPointerDown(const GameEvent& e) {}

  /** @brief Triggered when the pointer/mouse button is released. */
  virtual void OnPointerUp(const GameEvent& e) {}

  /** @brief Triggered when the pointer enters the element's bounding box.
   * Default behavior: Dims the color to provide visual feedback. */
  virtual void OnPointerEnter(const GameEvent& e) { SetColor(sf::Color(200, 200, 200)); }

  /** @brief Triggered when the pointer leaves the element's bounding box.
   * Default behavior: Resets the color to white. */
  virtual void OnPointerExit(const GameEvent& e) { SetColor(sf::Color::White); }

  /** @brief Triggered when the pointer moves while inside the element. */
  virtual void OnPointerMove(const GameEvent& e) {}

  UIInteractable* m_interactable = nullptr;  ///< Logic component handling hit detection and states.
  std::string p_actionID = "Default";        ///< Identifier for the command or action triggered by this element.
};

}  // namespace FalkonEngine