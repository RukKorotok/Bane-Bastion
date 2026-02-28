#pragma once
#include <string>

#include "GameWorld.h"
#include "SubScriptionSystem.h"

namespace FalkonEngine {

/**
 * @brief Abstract base class representing a single game state or level.
 * Each scene owns its own GameWorld and acts as an Observer to respond
 * to global game events.
 */
class Scene : public Observer {
 public:
  /**
   * @brief Constructs a scene with a unique identifier.
   * @param name The display or internal name of the scene.
   */
  Scene(const std::string& name);

  /**
   * @brief Virtual destructor to ensure proper cleanup of the GameWorld and entities.
   */
  virtual ~Scene();

  /** @return Pointer to the currently active scene managed by the SceneManager. */
  static Scene* GetActive();

  /** @return Pointer to the GameWorld instance belonging to this scene. */
  GameWorld* GetWorld() const;

  /** @return The name identifier of the scene. */
  std::string GetName();

  /**
   * @brief Implementation of the Observer interface to handle scene-specific events.
   * @param event The triggered GameEvent.
   */
  void OnNotify(const GameEvent& event) override;

  // --- (Lifecycle Methods: To be implemented by specific levels) ---

  /** @brief Called when the scene is loaded into the stack. Used for asset initialization. */
  virtual void Start() = 0;

  /** @brief Resets the scene state without fully reloading assets. */
  virtual void Restart() = 0;

  /** @brief Called before the scene is destroyed or popped from the stack. */
  virtual void Stop() = 0;

 protected:
  GameWorld* m_world = nullptr;  ///< Every scene manages its own entity-component world.

 private:
  std::string m_name;  ///< Internal name for debugging and identification.
};

}  // namespace FalkonEngine