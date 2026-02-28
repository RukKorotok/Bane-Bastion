#include "pch.h"

#include "ResourceSystem.h"
#include "SceneManager.h"

namespace FalkonEngine {

// SceneManager --- Singleton instance for navigation and scene lifetime management ---
//--------------------------------------------------------------------------------------------------------
SceneManager& SceneManager::Instance() {
  static SceneManager instance;
  return instance;
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::RegisterScene(const std::string& name, SceneFactory factory) {
  m_factories[name] = factory;
  FE_CORE_INFO("SceneManager: Registered factory for '" + name + "'");
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::OnNotify(const GameEvent& event) {
  switch (event.type) {
    case GameEventType::SceneLoadRequest:
      // FUTURE: Handle scene name passing via event data (e.g., event.message)
      // ApplyLoadScene(event.message);
      break;

    case GameEventType::ScenePopRequest:
      ApplyPopScene();
      break;
  }
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::ApplyLoadScene(const std::string& name) {
  auto it = m_factories.find(name);
  if (it != m_factories.end()) {
    ChangeSceneInternal(it->second());
  } else {
    FE_CORE_ERROR("SceneManager: Scene factory not found: " + name);
  }
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::Clear() {
  FE_CORE_INFO("SceneManager: Shutdown cleanup...");
  ChangeSceneInternal(nullptr);
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::ChangeSceneInternal(std::unique_ptr<Scene> newScene) {
  m_isLoading = true;

  // 1. LIFECYCLE: Stop and destroy all current scenes in the stack.
  while (!m_sceneStack.empty()) {
    m_sceneStack.top()->Stop();
    m_sceneStack.pop();
    // IMPORTANT: pop() destroys the unique_ptr, triggering
    // Scene -> World -> GameObjects destruction sequence.
  }

  // 2. MEMORY: Now that all old objects are guaranteed dead and textures are unused,
  // we can safely purge the resource cache.
  ResourceSystem::Instance()->Clear();

  // 3. INITIALIZATION: Move and start the new scene.
  if (newScene) {
    m_sceneStack.push(std::move(newScene));
    m_sceneStack.top()->Start();  // New textures are loaded here
  }

  m_isLoading = false;
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::ApplyPushScene(const std::string& name) {
  auto it = m_factories.find(name);
  if (it == m_factories.end()) {
    FE_CORE_ERROR("SceneManager: Cannot push scene. Factory for '" + name + "' not found!");
    return;
  }

  FE_CORE_INFO("SceneManager: Pushing overlay scene '" + name + "'");

  // OVERLAY LOGIC: We do NOT clear resources or pop the current scene.
  // The new scene is layered on top, allowing for UI menus or pauses.
  std::unique_ptr<Scene> overlayScene = it->second();
  overlayScene->Start();

  m_sceneStack.push(std::move(overlayScene));
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::ApplyPopScene() {
  if (m_sceneStack.size() > 1) {
    m_sceneStack.top()->Stop();
    m_sceneStack.pop();
  }
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::Update(float deltaTime) {
  if (!m_sceneStack.empty() && !m_isLoading) {
    // PAUSE LOGIC: Only the top-most scene receives update ticks.
    auto* world = m_sceneStack.top()->GetWorld();
    if (world) {
      world->Update(deltaTime);
      world->FixedUpdate(deltaTime);
      world->LateUpdate();
    }
  }
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::Render() {
  if (m_sceneStack.empty()) {
    FE_CORE_ERROR("Render: Stack is empty!");
    return;
  }

  if (m_isLoading) {
    FE_CORE_WARN("Render: Still loading...");
    return;
  }

  // RENDERING: Draw the current active scene.
  auto* activeScene = m_sceneStack.top().get();
  if (activeScene && activeScene->GetWorld()) {
    activeScene->GetWorld()->Render();
  }
}

//--------------------------------------------------------------------------------------------------------
Scene* SceneManager::GetActiveScene() const { return m_sceneStack.empty() ? nullptr : m_sceneStack.top().get(); }

}  // namespace FalkonEngine