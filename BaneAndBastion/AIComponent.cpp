#include "AIComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "pch.h"

namespace BaneAndBastion {
AIComponent::AIComponent(FalkonEngine::GameObject* gameObject)
    : FalkonEngine::Component(gameObject) {}

void AIComponent::Init(FalkonEngine::GameObject* target, float range,
                       std::unique_ptr<IPathfindingStrategy> strategy,
                       FalkonEngine::CollisionCategory collision) {
  m_targetPlayer = target;
  m_detectionRange = range;
  m_strategy = std::move(strategy);
  m_collision = collision;
}

void AIComponent::UpdateTarget(FalkonEngine::GameObject* target) {
  m_targetPlayer = target;
}

void AIComponent::Update(float dt) {
  if (!m_strategy || !m_targetPlayer) return;

  auto myTransform =
      GetGameObject()->GetComponent<FalkonEngine::TransformComponent>();
  auto playerTransform =
      m_targetPlayer->GetComponent<FalkonEngine::TransformComponent>();

  if (!myTransform || !playerTransform) return;

  FalkonEngine::Vector2Df myPos = myTransform->GetWorldPosition();
  FalkonEngine::Vector2Df playerPos = playerTransform->GetWorldPosition();

  // 1. Check detection range
  if ((playerPos - myPos).GetLength() < m_detectionRange) {
    // 2. Request path from strategy (considering this NPC's collisions)
    auto path = m_strategy->GetPath(myPos, playerPos, m_collision);

    // Index [1] is required because index [0] is the current position
    if (path.size() > 1) {
      auto nextPoint = path[1];

      // Minimum displacement check to prevent "jittering"
      if (nextPoint.GetLength() < 0.001f) return;

      // 3. Create a movement request event
      FalkonEngine::GameEvent event;
      event.type = FalkonEngine::GameEventType::MovementRequested;
      event.sender = this;
      event.entityID = GetGameObject()->GetID();  // Added ID for logging
      event.direction = nextPoint;

      // Notify subscribers (e.g., MoveComponent)
      this->Notify(event);

      FE_APP_TRACE("AIComponent: NPC " + std::to_string(event.entityID) +
                   " heading to (" + std::to_string(nextPoint.x) + ", " +
                   std::to_string(nextPoint.y) + ")");
    }
  }
}
}  // namespace BaneAndBastion