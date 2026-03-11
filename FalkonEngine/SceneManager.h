#pragma once

#include <functional>
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "Scene.h"
#include "SubScriptionSystem.h"

namespace FalkonEngine {

/**
 * @brief Function type for scene creation (Factory pattern).
 * Allows lazy instantiation of scenes only when they are needed.
 */
using SceneFactory = std::function<std::unique_ptr<Scene>()>;

/**
 * @brief Singleton manager responsible for scene transitions and lifecycle.
 * Supports both switching scenes (clearing stack) and pushing/popping scenes
 * for UI overlays or sub-menus.
 */
class SceneManager : public Observer {
 public:
  /** @brief Returns the global instance of the SceneManager. */
  static SceneManager& Instance();

  /**
   * @brief Registers a scene "blueprint" in the manager.
   * @param name Unique identifier for the scene.
   * @param factory Lambda or function that creates the Scene instance.
   */
  void RegisterScene(const std::string& name, SceneFactory factory);

  /**
   * @brief Processes navigation events (SceneLoadRequest, ScenePopRequest, etc.).
   * Part of the Observer pattern implementation.
   */
  void OnNotify(const GameEvent& event) override;

  /** @brief Updates the currently active scene. Called from the main engine loop. */
  void Update(float deltaTime);

  /** @brief Renders the current scene and potentially underlying stacked scenes. */
  void Render();

  /** @return Pointer to the top-most active scene in the stack. */
  Scene* GetActiveScene() const;

  /**
   * @brief Immediately replaces the current scene stack with a new scene.
   * @param name Registered name of the scene to load.
   */
  void ApplyLoadScene(const std::string& name);

  /** @brief Destroys all scenes and clears the stack. */
  void Clear();

  /** @brief Pushes a new scene onto the stack without removing previous ones. */
  void ApplyPushScene(const std::string& name);

  /** @brief Removes the top-most scene and returns to the previous one. */
  void ApplyPopScene();

 private:
  SceneManager() = default;
  ~SceneManager() = default;

  // Singleton: Prevent copying
  SceneManager(const SceneManager&) = delete;
  SceneManager& operator=(const SceneManager&) = delete;

  /** @brief Internal helper to swap the top scene or replace the stack. */
  void ChangeSceneInternal(std::unique_ptr<Scene> newScene);

  std::map<std::string, SceneFactory> m_factories;  ///< Registry of available scene types.
  std::vector<std::unique_ptr<Scene>> m_sceneStack;  ///< The actual stack of active scenes.
  bool m_isLoading = false;                         ///< Guard flag for transition safety.
};

}  // namespace FalkonEngine