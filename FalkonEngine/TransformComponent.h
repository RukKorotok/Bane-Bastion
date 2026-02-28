#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Component.h"
#include "GameObject.h"
#include "Matrix2D.h"
#include "Vector.h"

namespace FalkonEngine {

/**
 * @brief Component that defines the position, rotation, and scale of a GameObject.
 * Supports hierarchical parenting, allowing transforms to be calculated relative
 * to a parent object or in absolute world space using matrix math.
 */
class TransformComponent : public Component {
 public:
  /**
   * @brief Constructs a TransformComponent for a specific GameObject.
   * @param gameObject Pointer to the owner GameObject.
   */
  TransformComponent(GameObject* gameObject);

  /**
   * @brief Logic update for the transform (if needed).
   * @param deltaTime Time elapsed since the last frame.
   */
  void Update(float deltaTime) override;

  /**
   * @brief Optional debug rendering of the transform (e.g., axes).
   */
  void Render() override;

  // --- (Position) ---

  /** @brief Moves the object by a given offset in world space. */
  void MoveBy(const Vector2Df& offset);

  /** @brief Moves the object by X and Y offsets in world space. */
  void MoveBy(float offsetX, float offsetY);

  /** @brief Sets the absolute position of the object in world space. */
  void SetWorldPosition(const Vector2Df& position);
  void SetWorldPosition(float positionX, float positionY);

  /** @brief Sets the position of the object relative to its parent. */
  void SetLocalPosition(const Vector2Df& position);
  void SetLocalPosition(float positionX, float positionY);

  // --- (Rotation) ---

  /** @brief Rotates the object by an added angle (in degrees). */
  void RotateBy(float offset);

  /** @brief Sets the absolute rotation of the object in world space. */
  void SetWorldRotation(float angle);

  /** @brief Sets the rotation of the object relative to its parent. */
  void SetLocalRotation(float angle);

  // --- (Scale) ---

  /** @brief Scales the object by a given offset. */
  void ScaleBy(const Vector2Df& scaleOffset);

  /** @brief Scales the object by X and Y offsets. */
  void ScaleBy(float scaleX, float scaleY);

  /** @brief Sets the absolute scale of the object in world space. */
  void SetWorldScale(const Vector2Df& scale);
  void SetWorldScale(float scaleX, float scaleY);

  /** @brief Sets the scale of the object relative to its parent. */
  void SetLocalScale(const Vector2Df& scale);
  void SetLocalScale(float scaleX, float scaleY);

  // --- (Getters) ---

  const Vector2Df& GetWorldPosition() const;
  const Vector2Df& GetLocalPosition() const;

  const float GetWorldRotation() const;
  const float GetLocalRotation() const;

  const Vector2Df& GetWorldScale() const;
  const Vector2Df& GetLocalScale() const;

  // --- (Parenting Hierarchy) ---

  /** * @brief Sets a parent transform.
   * This object will now move, rotate, and scale relative to the parent.
   */
  void SetParent(TransformComponent* newParent);

  /** @return Pointer to the current parent transform, or nullptr if none. */
  TransformComponent* GetParent() const;

  /** * @brief Calculates and returns the final transformation matrix in world space.
   */
  const Matrix2D GetWorldTransform() const;

  // --- (Debug) ---

  /** @brief Prints transform data (pos, rot, scale) to the console/logger. */
  void Print() const;

 private:
  TransformComponent* m_parent = nullptr;  ///< Parent in the transform hierarchy.

  // Lazy Evaluation and Caching
  mutable Matrix2D m_localTransform;  ///< Cached local transformation matrix.
  mutable bool m_isUpdated = false;   ///< Dirty flag: true if world info is up to date.

  mutable Vector2Df m_localPosition = {0, 0};
  mutable float m_localRotation = 0.f;
  mutable Vector2Df m_localScale = {1, 1};

  mutable Vector2Df m_position = {0, 0};  ///< Calculated world position.
  mutable float m_rotation = 0.f;         ///< Calculated world rotation.
  mutable Vector2Df m_scale = {1, 1};     ///< Calculated world scale.

  // Internal helper methods for matrix decomposition and updates
  void setWorldInfoFrom(const Matrix2D& transform) const;
  void setLocalInfoFrom(const Matrix2D& transform) const;

  void updateLocalTransform() const;
  void updateLocalTransform(const Vector2Df& position, float rotation, const Vector2Df& scale) const;

  Matrix2D createTransform(const Vector2Df& position, float rotation, const Vector2Df& scale) const;

  /** @brief Marks the transform as "dirty", forcing a recalculation on the next access. */
  void Invalidate();
};

}  // namespace FalkonEngine