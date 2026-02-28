#pragma once

#include "CollisionCategories.h"
#include "Entity.h"
#include "GameObject.h"
#include "ResourceSystem.h"
#include "SubScriptionSystem.h"

namespace FalkonEngine {

/**
 * @brief High-level logical wrapper for game entities.
 * Acts as a bridge between the raw GameObject/Component system and specific
 * gameplay logic. Inherits from Observer and Observable to participate in
 * the global messaging system.
 */
class Actor : public Observer, public Observable {
 public:
  /**
   * @brief Constructs an Actor and prepares its base data.
   * @param position Initial world position.
   * @param name Unique name for the underlying GameObject.
   * @param texture Path to the texture asset.
   * @param collision The collision category for physics filtering.
   */
  Actor(Vector2Df position, const std::string& name, std::string texture, CollisionCategory collision);

  /** @brief Virtual destructor. Destruction is typically handled via the GameWorld. */
  virtual ~Actor() = default;

  /**
   * @brief Initializes the Actor, creating the GameObject and attaching
   * necessary default components (Transform, Sprite, etc.).
   */
  void Init();

  /**
   * @brief Standard Observer callback. To be overridden by specific actor types
   * (e.g., Player, Enemy) to respond to game events.
   */
  void OnNotify(const FalkonEngine::GameEvent& event) override {};

  /**
   * @brief Marks the Actor and its underlying GameObject for removal
   * from the GameWorld.
   */
  void Destroy();

  /** @return Pointer to the managed GameObject instance. */
  FalkonEngine::GameObject* GetGameObject() const;

  /** @return The current collision category of this actor. */
  virtual CollisionCategory GetCollisionCategory() const;

  /**
   * @brief Updates the collision category for physics filtering.
   * @param collision New collision category to apply.
   */
  void SetCollition(CollisionCategory collision);

 protected:
  FalkonEngine::GameObject* p_gameObject = nullptr;         ///< The entity in the GameWorld managed by this actor.
  bool p_isDestroyed = false;                               ///< Flag to prevent multiple destruction calls.
  std::string p_name = "";                                  ///< Debug and identification name.
  CollisionCategory p_collision = CollisionCategory::None;  ///< Physics layer/mask category.
};

}  // namespace FalkonEngine