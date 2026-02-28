#include "pch.h"

#include "PhysicsSystem.h"

namespace FalkonEngine {

// PhysicsSystem --- Implementation of collision detection and resolution ---
//-----------------------------------------------------------------------------------------------------------
PhysicsSystem* PhysicsSystem::Instance() {
  static PhysicsSystem physicsSystem;
  return &physicsSystem;
}

//-----------------------------------------------------------------------------------------------------------
float PhysicsSystem::GetFixedDeltaTime() const { return m_fixedDeltaTime; }

//-----------------------------------------------------------------------------------------------------------
void PhysicsSystem::Update() {
  // 1. BROAD & NARROW PHASE: Check every registered collider against others
  for (size_t i = 0; i < m_colliders.size(); i++) {
    if (!m_colliders[i]) continue;

    // Optimization: Only move objects that have a Rigidbody and are not Kinematic
    auto body = m_colliders[i]->GetGameObject()->GetComponent<RigidbodyComponent>();
    if (body == nullptr || body->GetKinematic()) continue;

    auto catA = m_colliders[i]->GetCategory();

    for (size_t j = 0; j < m_colliders.size(); j++) {
      if (i == j || !m_colliders[j]) continue;

      auto catB = m_colliders[j]->GetCategory();

      // FILTERING: Use CollisionRegistry to skip ignored layers
      if (!CollisionRegistry::ShouldCollide(catA, catB)) continue;

      sf::FloatRect intersection;
      if (m_colliders[i]->m_bounds.intersects(m_colliders[j]->m_bounds, intersection)) {
        // CASE A: TRIGGER (No physical push, just events)
        if (m_colliders[i]->m_isTrigger || m_colliders[j]->m_isTrigger) {
          if (m_triggersEnteredPair.find(m_colliders[i]) == m_triggersEnteredPair.end()) {
            Trigger triggerInfo(m_colliders[i], m_colliders[j]);
            m_colliders[i]->OnTriggerEnter(triggerInfo);
            m_colliders[j]->OnTriggerEnter(triggerInfo);

            m_triggersEnteredPair.emplace(m_colliders[i], m_colliders[j]);
            FE_APP_TRACE("Trigger Enter: " + m_colliders[i]->GetGameObject()->GetName() + " <-> " +
                         m_colliders[j]->GetGameObject()->GetName());
          }
        }
        // CASE B: SOLID COLLISION (Physical resolution)
        else {
          float intersectionWidth = intersection.width;
          float intersectionHeight = intersection.height;

          Vector2Df aCenter(m_colliders[i]->m_bounds.left + m_colliders[i]->m_bounds.width / 2.f,
                            m_colliders[i]->m_bounds.top + m_colliders[i]->m_bounds.height / 2.f);
          Vector2Df bCenter(m_colliders[j]->m_bounds.left + m_colliders[j]->m_bounds.width / 2.f,
                            m_colliders[j]->m_bounds.top + m_colliders[j]->m_bounds.height / 2.f);

          auto aTransform = m_colliders[i]->GetGameObject()->GetComponent<TransformComponent>();

          // RESOLUTION: Push the object out along the shortest axis
          if (intersectionWidth > intersectionHeight) {
            if (aCenter.y < bCenter.y) {
              aTransform->MoveBy({0.f, -intersectionHeight});  // Push Up
            } else {
              aTransform->MoveBy({0.f, intersectionHeight});  // Push Down
            }
          } else {
            if (aCenter.x < bCenter.x) {
              aTransform->MoveBy({-intersectionWidth, 0.f});  // Push Left
            } else {
              aTransform->MoveBy({intersectionWidth, 0.f});  // Push Right
            }
          }

          Collision collisionInfo(m_colliders[i], m_colliders[j], intersection);
          m_colliders[i]->OnCollision(collisionInfo);
          m_colliders[j]->OnCollision(collisionInfo);
        }
      }
    }
  }

  // 2. TRIGGER EXIT PASS: Detect when objects stop overlapping
  for (auto it = m_triggersEnteredPair.begin(); it != m_triggersEnteredPair.end();) {
    if (!it->first->m_bounds.intersects(it->second->m_bounds)) {
      Trigger exitInfo(it->first, it->second);
      it->first->OnTriggerExit(exitInfo);
      it->second->OnTriggerExit(exitInfo);

      FE_APP_TRACE("Trigger Exit: " + it->first->GetGameObject()->GetName());
      it = m_triggersEnteredPair.erase(it);
    } else {
      ++it;
    }
  }
}

//-----------------------------------------------------------------------------------------------------------
void PhysicsSystem::Subscribe(ColliderComponent* collider) {
  if (!collider) return;
  m_colliders.push_back(collider);
  FE_APP_TRACE("PhysicsSystem: Collider subscribed (" + collider->GetGameObject()->GetName() + ")");
}

//-----------------------------------------------------------------------------------------------------------
void PhysicsSystem::Unsubscribe(ColliderComponent* collider) {
  if (!collider) return;

  // 1. Clean main collider list
  auto it = std::remove(m_colliders.begin(), m_colliders.end(), collider);
  if (it != m_colliders.end()) {
    m_colliders.erase(it, m_colliders.end());
  }

  // 2. Clean trigger tracking pairs
  for (auto tIt = m_triggersEnteredPair.begin(); tIt != m_triggersEnteredPair.end();) {
    if (tIt->first == collider || tIt->second == collider) {
      tIt = m_triggersEnteredPair.erase(tIt);
    } else {
      ++tIt;
    }
  }
}
}  // namespace FalkonEngine