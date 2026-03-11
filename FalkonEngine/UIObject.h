#pragma once

#include "GameObject.h"
#include "RectTransformComponent.h"

namespace FalkonEngine {

/**
 * @brief Defines the direction in which a UI element (like a progress bar) fills up.
 */
enum class FillDirection { LeftToRight, RightToLeft, BottomToTop, TopToBottom };

/**
 * @brief Base class for all UI-based GameObjects.
 * Automatically manages a RectTransformComponent and provides utilities for
 * coordinate-based hit detection and UI-specific spatial properties.
 */
class UIObject : public GameObject {
 public:
  /**
   * @brief Default constructor. Initializes the object as a UI entity.
   */
  UIObject();

  /**
   * @brief Constructs a UIObject with a specific name.
   * @param name The name of the UI element.
   */
  UIObject(std::string name);

  /**
   * @brief Virtual destructor.
   */
  virtual ~UIObject() {}

  /**
   * @brief Initializes the UIObject and ensures a RectTransformComponent is attached.
   */
  void Awake() override;

  /**
   * @brief Updates UI logic.
   * @param deltaTime Time elapsed since the last frame.
   */
  void Update(float deltaTime);

  /**
   * @brief UI rendering is typically handled by specialized components (like UIImage).
   */
  void Render() {};

  /**
   * @brief Checks if a world or screen-space point lies within the UI element's rectangle.
   * @param point The coordinates to test.
   * @return True if the point is inside the bounds.
   */
  bool IsPointInside(Vector2Df point) const;

  /**
   * @brief Sets the size of the UI element.
   * @param size Dimensions in pixels.
   */
  virtual void SetSize(Vector2Df size);

  /**
   * @return The current size from the RectTransform.
   */
  Vector2Df GetSize() const;

  /**
   * @brief Sets the pivot point (normalized 0.0 to 1.0).
   * @param pivot The new origin point of the rectangle.
   */
  virtual void SetPivot(Vector2Df pivot);

  /**
   * @return The current pivot point from the RectTransform.
   */
  Vector2Df GetPivot() const;

 protected:
  /**
   * @brief Internal event dispatcher for UI-specific game events.
   * @param event The event data.
   */
  void HandleEvent(const GameEvent& event) override;

 private:
  RectTransformComponent* m_rectTransform = nullptr;  ///< Cached reference to the UI transform.
};

}  // namespace FalkonEngine