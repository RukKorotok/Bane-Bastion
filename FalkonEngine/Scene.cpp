#include "pch.h"

#include "Scene.h"
#include "SceneManager.h"

namespace FalkonEngine {
// Scene
//-----------------------------------------------------------------------------------------------------------
Scene::Scene(const std::string& name) : m_name(name), m_world(nullptr) {
  try {
    m_world = new GameWorld();
    m_world->Subscribe(this);
  } catch (const std::exception& e) {
    FE_CORE_ERROR("Failed to allocate GameWorld for scene '" + m_name +
                  "': " + e.what());
    throw;
  }

  FE_CORE_INFO("Scene '" + m_name + "' created.");
}
//-----------------------------------------------------------------------------------------------------------
Scene::~Scene() {
  FE_CORE_INFO("Destroying scene: " + m_name);

  delete m_world;
  FE_APP_TRACE("Scene" + m_name + "remover");
}
//-----------------------------------------------------------------------------------------------------------
Scene* Scene::GetActive() { return SceneManager::Instance().GetActiveScene(); }
//-----------------------------------------------------------------------------------------------------------
GameWorld* Scene::GetWorld() const {
  FE_CORE_ASSERT(m_world != nullptr,
                 "GameWorld in scene '" + m_name + "' is null!");

  return m_world;
}
//-----------------------------------------------------------------------------------------------------------
std::string Scene::GetName() { return m_name; }
//-----------------------------------------------------------------------------------------------------------
void Scene::OnNotify(const GameEvent& event) {
  FE_APP_TRACE("Scene '" + m_name + "': Notified of object removal.");
}
}  // namespace FalkonEngine