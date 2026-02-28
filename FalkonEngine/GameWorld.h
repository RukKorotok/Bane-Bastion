#pragma once

#include <type_traits>
#include <vector>
#include "GameObject.h"
#include "PhysicsSystem.h"

namespace FalkonEngine {

/**
 * @brief Container and manager for all GameObjects in a specific Scene.
 * Handles the update loop (Update, FixedUpdate, LateUpdate), rendering order,
 * and memory management of entities. Inherits from Observable to broadcast
 * world state changes.
 */
class GameWorld : public Observable {
 public:
  /** @brief Initializes an empty game world. */
  GameWorld() = default;

  /** @brief Cleans up all GameObjects and associated resources. */
  ~GameWorld();

  // Singleton-like world management: Prevent copying to avoid double-freeing objects
  GameWorld(GameWorld const&) = delete;
  GameWorld& operator=(GameWorld const&) = delete;

  /** @brief Standard frame update for logic and animations. */
  void Update(float deltaTime);

  /** @brief Physics-consistent update called at a fixed time interval. */
  void FixedUpdate(float deltaTime);

  /** @brief Renders all objects, separating world-space entities from UI. */
  void Render();

  /** @brief Called after Update to handle camera following or final transformations. */
  void LateUpdate();

  /**
   * @brief Factory method to instantiate and register a new GameObject.
   * @tparam T The type of GameObject to create (must derive from GameObject).
   * @param args Arguments to pass to the object's constructor.
   * @return Pointer to the newly created and initialized object.
   */
  template <typename T, typename... Args>
  T* CreateGameObject(Args&&... args) {
    static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");

    T* newObject = new T(std::forward<Args>(args)...);
    m_gameObjects.push_back(newObject);

    // INITIALIZATION: Awake is called immediately after construction
    newObject->Awake();

    // LAYER SORTING: Distinguish between standard world objects and UI elements
    if (newObject->GetLayer() == RenderLayer::UI) {
      m_uiLayer.push_back(newObject);
    } else {
      m_worldLayer.push_back(newObject);
    }

    return newObject;
  }

  /**
   * @brief Marks an object for destruction at the end of the current frame.
   * @param gameObject Pointer to the object to be removed.
   */
  void DestroyGameObject(GameObject* gameObject);

  /** @brief Immediately destroys all objects in the world. */
  void Clear();

  /** @brief Debug utility to print the current world hierarchy/state. */
  void Print() const;

 private:
  float m_fixedCounter = 0.f;  ///< Accumulator for fixed time-step logic.

  std::vector<GameObject*> m_gameObjects = {};                 ///< Main registry of all objects.
  std::vector<GameObject*> m_markedToDestroyGameObjects = {};  ///< Queue for deferred destruction.

  // Rendering optimization layers
  std::vector<GameObject*> m_worldLayer = {};  ///< Standard objects (sprites, players, etc.).
  std::vector<GameObject*> m_uiLayer = {};     ///< Screen-space overlays (buttons, bars).

  /** @brief Internal logic to perform memory cleanup and pointer removal. */
  void DestroyGameObjectImmediate(GameObject* gameObject);
};

}  // namespace FalkonEngine