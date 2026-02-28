#include "pch.h"

#include "RigidbodyComponent.h"

#include <cmath>

namespace FalkonEngine {

// RigidbodyComponent --- Physical movement and velocity integration ---
//-----------------------------------------------------------------------------------------------------------
RigidbodyComponent::RigidbodyComponent(GameObject* gameObject) : Component(gameObject) {
  m_transform = gameObject->GetComponent<TransformComponent>();

  // DEPENDENCY CHECK: Physics simulation requires a Transform for position updates
  FE_CORE_ASSERT(m_transform != nullptr, "RigidbodyComponent added to GameObject '" + p_gameObject->GetName() +
                                             "' without TransformComponent!");

  FE_APP_TRACE("RigidbodyComponent initialized for: " + p_gameObject->GetName());
}

//-----------------------------------------------------------------------------------------------------------
void RigidbodyComponent::Update(float deltaTime) {
  if (deltaTime <= 0.0f) {
    return;
  }

  // BROADCAST: Send velocity requests to the event bus for translation/collision resolution
  FalkonEngine::GameEvent moveEv;
  moveEv.type = FalkonEngine::GameEventType::HitVelocityRequest;
  moveEv.entityID = GetGameObject()->GetID();
  moveEv.direction = m_linearVelocity;
  moveEv.value = deltaTime;
  moveEv.sender = this;
  Notify(moveEv);

  // BROADCAST: Send rotation requests based on current angular velocity
  FalkonEngine::GameEvent rotateEv;
  rotateEv.type = FalkonEngine::GameEventType::HitRotationRequest;
  rotateEv.entityID = GetGameObject()->GetID();
  rotateEv.angle = m_angleVelocity;
  rotateEv.value = deltaTime;
  rotateEv.sender = this;
  Notify(rotateEv);

  // DAMPING: Apply linear and angular drag to simulate air/surface resistance
  float linearDrag = 1.f - m_linearDamping * deltaTime;
  float angleDrag = 1.f - m_angleDamping * deltaTime;

  m_linearVelocity = (linearDrag > 0.f ? linearDrag : 0.f) * m_linearVelocity;
  m_angleVelocity = (angleDrag > 0.f ? angleDrag : 0.f) * m_angleVelocity;

  // EPSILON CHECKS: Stop micro-movements to prevent physics jitter and save performance
  if (std::abs(m_linearVelocity.x) < 0.1f) {
    m_linearVelocity.x = 0.0f;
  }
  if (std::abs(m_linearVelocity.y) < 0.1f) {
    m_linearVelocity.y = 0.0f;
  }

  if (std::abs(m_angleVelocity) < 0.001f) {
    m_angleVelocity = 0.0f;
  }
}

//-----------------------------------------------------------------------------------------------------------
void RigidbodyComponent::Render() {
  // Optional: Visual representation of velocity vectors for debugging
}

//-----------------------------------------------------------------------------------------------------------
void RigidbodyComponent::SetLinearVelocity(Vector2Df newLinearVelocity) {
  // VALIDATION: Ensure physics values remain within finite bounds
  if (!std::isfinite(newLinearVelocity.x) || !std::isfinite(newLinearVelocity.y)) {
    FE_CORE_ERROR("Attempted to set non-finite LinearVelocity on: " + p_gameObject->GetName());
    return;
  }
  m_linearVelocity = newLinearVelocity;
}

//-----------------------------------------------------------------------------------------------------------
void RigidbodyComponent::AddLinearVelocity(Vector2Df linearOffset) {
  m_linearVelocity = m_linearVelocity + linearOffset;
}

//-----------------------------------------------------------------------------------------------------------
Vector2Df RigidbodyComponent::GetLinearVelocity() const { return m_linearVelocity; }

//-----------------------------------------------------------------------------------------------------------
void RigidbodyComponent::SetAngleVelocity(float newAngleVelocity) {
  if (!std::isfinite(newAngleVelocity)) {
    FE_CORE_ERROR("Attempted to set non-finite AngleVelocity on: " + p_gameObject->GetName());
    return;
  }
  m_angleVelocity = newAngleVelocity;
}

void RigidbodyComponent::AddAngleVelocity(float angleOffset) { m_angleVelocity += angleOffset; }

float RigidbodyComponent::GetAngleVelocity() const { return m_angleVelocity; }

//-----------------------------------------------------------------------------------------------------------
void RigidbodyComponent::SetLinearDamping(float newLinearDamping) { m_linearDamping = newLinearDamping; }

float RigidbodyComponent::GetLinearDamping() const { return m_linearDamping; }

//-----------------------------------------------------------------------------------------------------------
void RigidbodyComponent::SetAngleDamping(float newAngleDamping) { m_angleDamping = newAngleDamping; }

float RigidbodyComponent::GetAngleDamping() const { return m_angleDamping; }

//-----------------------------------------------------------------------------------------------------------
void RigidbodyComponent::SetKinematic(bool newIsKinematic) { m_isKinematic = newIsKinematic; }

//-----------------------------------------------------------------------------------------------------------
bool RigidbodyComponent::GetKinematic() const { return m_isKinematic; }

//-----------------------------------------------------------------------------------------------------------
void RigidbodyComponent::ApplyImpulse(Vector2Df direction, float force) {
  float lenSq = direction.x * direction.x + direction.y * direction.y;

  if (lenSq > 0.000001f) {
    // IMPULSE LOGIC: Normalize direction and apply a sudden velocity change
    float len = std::sqrt(lenSq);
    Vector2Df normalizedDir = {direction.x / len, direction.y / len};
    AddLinearVelocity(normalizedDir * force);
  } else {
    FE_CORE_WARN("ApplyImpulse called with zero-length direction on: " + p_gameObject->GetName());
  }
}

}  // namespace FalkonEngine