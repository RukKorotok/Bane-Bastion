#include "pch.h"

#include "GameWorld.h"
#include "RenderSystem.h"
#include "Scene.h"
#include "SpriteColliderComponent.h"

namespace FalkonEngine {

// GameWorld --- Core logic for entity management and the main loop ---
//-----------------------------------------------------------------------------------------------------------
GameWorld::~GameWorld() {
  FE_APP_TRACE("GameWorld destructor called. Clearing all objects.");
  Clear();
}

//-----------------------------------------------------------------------------------------------------------
void GameWorld::Update(float deltaTime) {
  // LOOP: Standard frame update for all active game objects
  for (size_t i = 0; i < m_gameObjects.size(); i++) {
    if (m_gameObjects[i]) {
      m_gameObjects[i]->Update(deltaTime);
    }
  }
}

//-----------------------------------------------------------------------------------------------------------
void GameWorld::FixedUpdate(float deltaTime) {
  m_fixedCounter += deltaTime;
  float fixedStep = PhysicsSystem::Instance()->GetFixedDeltaTime();

  if (fixedStep <= 0.0001f) {
    FE_CORE_ASSERT(false, "Physics FixedDeltaTime is too small or zero!");
    return;
  }

  // ACCUMULATOR: Ensure physics ticks at a consistent rate regardless of frame rate
  if (m_fixedCounter > fixedStep) {
    m_fixedCounter -= fixedStep;
    PhysicsSystem::Instance()->Update();
  }
}

//-----------------------------------------------------------------------------------------------------------
void GameWorld::Render() {
  // 1. WORLD RENDER: Draw entities using the current world/camera view
  for (auto* obj : m_worldLayer) {
    if (obj) obj->Render();
  }

  // 2. UI RENDER: Reset view to default (screen space) to draw overlays
  sf::RenderWindow& window = RenderSystem::Instance()->GetMainWindow();
  window.setView(window.getDefaultView());

  for (auto* obj : m_uiLayer) {
    if (obj) obj->Render();
  }
}

//-----------------------------------------------------------------------------------------------------------
void GameWorld::LateUpdate() {
  // CLEANUP: Process objects marked for destruction at the very end of the frame
  if (!m_markedToDestroyGameObjects.empty()) {
    FE_APP_TRACE("LateUpdate: Destroying " + std::to_string(m_markedToDestroyGameObjects.size()) + " marked objects.");

    for (int i = static_cast<int>(m_markedToDestroyGameObjects.size()) - 1; i >= 0; i--) {
      DestroyGameObjectImmediate(m_markedToDestroyGameObjects[i]);
    }
    m_markedToDestroyGameObjects.clear();
  }
}

//-----------------------------------------------------------------------------------------------------------
void GameWorld::DestroyGameObject(GameObject* gameObject) {
  if (!gameObject) return;

  // DEFERRED DESTRUCTION: Add to queue to avoid invalidating iterators during Update
  auto it = std::find(m_markedToDestroyGameObjects.begin(), m_markedToDestroyGameObjects.end(), gameObject);

  if (it == m_markedToDestroyGameObjects.end()) {
    m_markedToDestroyGameObjects.push_back(gameObject);
  }
}

//-----------------------------------------------------------------------------------------------------------
void GameWorld::Clear() {
  FE_CORE_INFO("Clearing GameWorld...");

  // SAFE DESTRUCTION: Collect root objects first to prevent iteration conflicts
  std::vector<GameObject*> rootObjects;
  for (auto* obj : m_gameObjects) {
    if (obj) {
      auto transform = obj->GetComponent<TransformComponent>();
      if (transform && transform->GetParent() == nullptr) {
        rootObjects.push_back(obj);
      }
    }
  }

  // EXECUTION: Perform immediate destruction on the collected root objects
  for (auto* obj : rootObjects) {
    DestroyGameObjectImmediate(obj);
  }

  m_gameObjects.clear();
  m_markedToDestroyGameObjects.clear();
  m_fixedCounter = 0.0f;
}

//-----------------------------------------------------------------------------------------------------------
void GameWorld::Print() const {
  for (auto& obj : m_gameObjects) {
    if (obj == nullptr) continue;
    // Print hierarchy starting from root objects
    if (obj->GetComponent<TransformComponent>()->GetParent() == nullptr) {
      obj->Print();
    }
  }
}

//-----------------------------------------------------------------------------------------------------------
void GameWorld::DestroyGameObjectImmediate(GameObject* gameObject) {
  if (!gameObject) return;

  std::string objName = gameObject->GetName();

  // NOTIFICATION: Inform observers (like Scenes) that this object is being removed
  GameEvent confirmEv;
  confirmEv.type = FalkonEngine::GameEventType::ObjectRemoved;
  confirmEv.sender = this;
  confirmEv.object = gameObject;
  this->Notify(confirmEv);

  // UNBINDING: Detach from parent if it exists
  auto transform = gameObject->GetComponent<TransformComponent>();
  if (transform && transform->GetParent()) {
    transform->GetParent()->GetGameObject()->RemoveChild(gameObject);
  }

  // HIERARCHICAL DESTRUCTION: Gather all children to ensure the entire tree is deleted
  std::vector<TransformComponent*> allTransforms = gameObject->GetComponentsInChildren<TransformComponent>();

  FE_APP_TRACE("Immediate destruction of '" + objName + "' and its children.");

  auto removeFromLayer = [](std::vector<GameObject*>& layer, GameObject* target) {
    layer.erase(std::remove(layer.begin(), layer.end(), target), layer.end());
  };

  for (auto* t : allTransforms) {
    GameObject* obj = t->GetGameObject();
    if (!obj) continue;
    // Remove from all tracking registries
    m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), obj), m_gameObjects.end());
    m_markedToDestroyGameObjects.erase(
        std::remove(m_markedToDestroyGameObjects.begin(), m_markedToDestroyGameObjects.end(), obj),
        m_markedToDestroyGameObjects.end());

    removeFromLayer(m_worldLayer, obj);
    removeFromLayer(m_uiLayer, obj);

    delete obj;  // Final memory release
  }
}

}  // namespace FalkonEngine