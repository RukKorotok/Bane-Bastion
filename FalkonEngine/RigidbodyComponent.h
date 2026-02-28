#pragma once

#include "TransformComponent.h"
#include "Vector.h"

namespace FalkonEngine {

/**
 * @brief Component that enables physical movement for a GameObject.
 * Manages linear and angular velocities, damping, and kinematic state.
 * It directly modifies the GameObject's TransformComponent during the update cycle.
 */
class RigidbodyComponent : public Component {
 public:
  /**
   * @brief Constructs a RigidbodyComponent and caches the required TransformComponent.
   * @param gameObject Pointer to the owner GameObject.
   */
  RigidbodyComponent(GameObject* gameObject);

  /**
   * @brief Applies velocities and damping to the object's transform.
   * @param deltaTime Time elapsed since the last frame.
   */
  void Update(float deltaTime) override;

  /**
   * @brief Optional debug rendering for physics vectors.
   */
  void Render() override;

  /**
   * @brief Sets the current linear velocity vector.
   * @param newLinearVelocity The new velocity in units per second.
   */
  void SetLinearVelocity(Vector2Df newLinearVelocity);

  /**
   * @brief Offsets the current linear velocity.
   * @param linearOffset Vector to be added to the current velocity.
   */
  void AddLinearVelocity(Vector2Df linearOffset);

  /**
   * @return The current linear velocity vector.
   */
  Vector2Df GetLinearVelocity() const;

  /**
   * @brief Sets the current angular (rotation) velocity.
   * @param newAngleVelocity Rotation speed in degrees per second.
   */
  void SetAngleVelocity(float newAngleVelocity);

  /**
   * @brief Offsets the current angular velocity.
   * @param angleOffset Value to be added to the current rotation speed.
   */
  void AddAngleVelocity(float angleOffset);

  /**
   * @return The current angular velocity.
   */
  float GetAngleVelocity() const;

  /**
   * @brief Sets the linear damping (air resistance/friction).
   * @param newLinearDamping Damping factor (0 for no damping).
   */
  void SetLinearDamping(float newLinearDamping);

  /**
   * @return The current linear damping factor.
   */
  float GetLinearDamping() const;

  /**
   * @brief Sets the angular damping (rotational friction).
   * @param newAngleDamping Damping factor (0 for no damping).
   */
  void SetAngleDamping(float newAngleDamping);

  /**
   * @return The current angular damping factor.
   */
  float GetAngleDamping() const;

  /**
   * @brief Sets whether the object is kinematic.
   * Kinematic objects are not affected by velocities or forces automatically.
   * @param newIsKinematic True to make the object kinematic.
   */
  void SetKinematic(bool newIsKinematic);

  /**
   * @return True if the object is kinematic.
   */
  bool GetKinematic() const;

  /**
   * @brief Applies an instantaneous force (impulse) in a specific direction.
   * @param direction Normalized vector of the impulse.
   * @param force Magnitude of the impulse.
   */
  void ApplyImpulse(Vector2Df direction, float force);

 private:
  TransformComponent* m_transform;  ///< Cached pointer to the owner's transform.

  Vector2Df m_linearVelocity = {0.f, 0.f};  ///< Current movement velocity.
  float m_angleVelocity = 0.f;              ///< Current rotation velocity.

  float m_linearDamping = 0.f;  ///< Friction applied to linear movement.
  float m_angleDamping = 0.f;   ///< Friction applied to angular movement.

  bool m_isKinematic = false;  ///< If true, physics integration is skipped.
};

}  // namespace FalkonEngine