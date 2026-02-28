#pragma once

#include "Component.h"
#include "Vector.h"

namespace FalkonEngine {

/**
 * @brief Component specialized for UI elements and 2D rectangular layout.
 * Extends the basic transform logic with size and pivot (anchor) points,
 * facilitating UI alignment and hit detection.
 */
class RectTransformComponent : public Component {
 public:
  /**
   * @brief Constructs a RectTransformComponent for a specific GameObject.
   * @param gameObject Pointer to the owner GameObject.
   */
  RectTransformComponent(GameObject* gameObject);

  /**
   * @brief Default destructor.
   */
  virtual ~RectTransformComponent() = default;

  /**
   * @brief Sets the dimensions of the rectangular element in pixels.
   * @param size Vector2Df representing width and height.
   */
  void SetSize(Vector2Df size);

  /**
   * @brief Gets the current size of the element.
   * @return Dimensions in pixels.
   */
  Vector2Df GetSize() const;

  /**
   * @brief Sets the pivot point of the element.
   * Defines the origin for rotation, scaling, and positioning.
   * (0,0 is Top-Left, 0.5, 0.5 is Center, 1,1 is Bottom-Right).
   * @param pivot Normalized coordinates (0.0f to 1.0f).
   */
  void SetPivot(Vector2Df pivot);

  /**
   * @brief Gets the current pivot point.
   * @return Normalized pivot coordinates.
   */
  Vector2Df GetPivot() const;

  /**
   * @brief Helper to check if a specific point is within the rectangle's bounds.
   * @param point The point to check (in local or screen coordinates depending on implementation).
   * @return True if the point is inside the boundaries.
   */
  bool Contains(Vector2Df point) const;

  /**
   * @brief This component is structural and does not require logic updates.
   */
  void Update(float deltaTime) override {}

  /**
   * @brief This component is structural and does not perform drawing.
   */
  void Render() override {}

 private:
  Vector2Df m_size;   ///< Dimensions of the UI element in pixels.
  Vector2Df m_pivot;  ///< Normalized origin point for the rectangle.
};

}  // namespace FalkonEngine