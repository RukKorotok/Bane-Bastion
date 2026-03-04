#pragma once

#include <map>
#include <memory>
#include <string>

#include "AnimState.h"
#include "Component.h"
#include "SpriteRendererComponent.h"

namespace FalkonEngine {

/**
 * @brief Component managing multiple animation states for a GameObject.
 * Interfaces with a SpriteRendererComponent to update texture rects and
 * handles state transitions, playback speed, and animation logic.
 */
class AnimationComponent : public Component {
 public:
  /**
   * @brief Constructs an AnimationComponent and links it to the owner's renderer.
   * @param gameObject Pointer to the owner GameObject.
   */
  AnimationComponent(GameObject* gameObject);

  /**
   * @brief Destructor.
   */
  virtual ~AnimationComponent();

  /**
   * @brief Advances the current animation state and synchronizes with the renderer.
   * @param deltaTime Time elapsed since the last frame.
   */
  void Update(float deltaTime) override;

  /**
   * @brief Empty render implementation (logic is handled during Update).
   */
  void Render() override {}

  /**
   * @brief Registers a new animation state.
   * @param name Unique identifier for the state (e.g., "Idle", "Walk").
   * @param state Shared pointer to the AnimState configuration.
   */
  void AddState(const std::string& name, std::shared_ptr<AnimState> state);

  /**
   * @brief Switches the active animation to a previously registered state.
   * @param name The name of the state to activate.
   */
  void SetState(const std::string& name);

  /**
   * @brief Retrieves the currently playing animation state.
   * @return Shared pointer to the active AnimState.
   */
  std::shared_ptr<AnimState> GetCurrentState() { return m_currentState; }

  /**
   * @brief Gets the identifier of the active animation state.
   * @return Reference to the state name string.
   */
  const std::string& GetCurrentStateName() const;

  /**
   * @brief Updates the playback speed multiplier of the currently active state.
   * @param speed New speed multiplier (1.0f is default).
   */
  void SetCurrentSpeed(float speed);

  /**
   * @brief Updates the playback speed multiplier for a specific state by name.
   * @param name The name of the target state.
   * @param speed New speed multiplier.
   */
  void SetStateSpeed(const std::string& name, float speed);

 private:
  SpriteRendererComponent* m_renderer = nullptr;               ///< Cached reference to the owner's SpriteRenderer.
  std::map<std::string, std::shared_ptr<AnimState>> m_states;  ///< Collection of all registered animation states.
  std::shared_ptr<AnimState> m_currentState = nullptr;         ///< Pointer to the currently active animation.
  std::string m_currentStateName = "";                         ///< Name of the active animation state.
};

}  // namespace FalkonEngine