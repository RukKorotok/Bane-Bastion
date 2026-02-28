#include "pch.h"

#include "Scene.h"
#include "SceneManager.h"

namespace FalkonEngine {

// Scene --- Base implementation for game states ---
//-----------------------------------------------------------------------------------------------------------
Scene::Scene(const std::string& name) : m_name(name), m_world(nullptr) {
  try {
    // WORLD INITIALIZATION: Each scene creates its own unique entity container.
    m_world = new GameWorld();

    // EVENT BINDING: The scene listens to its own world's events (like object creation/destruction).
    m_world->Subscribe(this);
  } catch (const std::exception& e) {
    FE_CORE_ERROR("Failed to allocate GameWorld for scene '" + m_name + "': " + e.what());
    throw;  // Re-throw to prevent running with a broken scene
  }

  FE_CORE_INFO("Scene '" + m_name + "' created.");
}

//-----------------------------------------------------------------------------------------------------------
Scene::~Scene() {
  FE_CORE_INFO("Destroying scene: " + m_name);

  // MEMORY MANAGEMENT: Deleting the world triggers the destruction of all managed GameObjects.
  delete m_world;

  FE_APP_TRACE("Scene " + m_name + " removed.");
}

//-----------------------------------------------------------------------------------------------------------
Scene* Scene::GetActive() { return SceneManager::Instance().GetActiveScene(); }

//-----------------------------------------------------------------------------------------------------------
GameWorld* Scene::GetWorld() const {
  // SAFETY CHECK: Ensure we never try to access a world that hasn't been initialized.
  FE_CORE_ASSERT(m_world != nullptr, "GameWorld in scene '" + m_name + "' is null!");

  return m_world;
}

//-----------------------------------------------------------------------------------------------------------
std::string Scene::GetName() { return m_name; }

//-----------------------------------------------------------------------------------------------------------
void Scene::OnNotify(const GameEvent& event) {
  // LOGGING: Scene-specific response to world events.
  FE_APP_TRACE("Scene '" + m_name + "': Notified of event.");
}

}  // namespace FalkonEngine