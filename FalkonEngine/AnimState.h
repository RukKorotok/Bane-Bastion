#pragma once

#include "Animation.h"

namespace FalkonEngine {

class AnimationComponent;

/**
 * @brief Represents a specific state within the AnimationComponent.
 * Encapsulates animation data, texture references, and playback logic
 * including timing, looping, and frame progression.
 */
class AnimState {
 public:
  /**
   * @brief Constructs an AnimState with a defined animation and texture.
   * @param anim The animation sequence data.
   * @param texture Pointer to the source atlas texture.
   * @param loop Whether the animation should restart after the last frame.
   */
  AnimState(const Animation& anim, const sf::Texture* texture, bool loop = true);

  /**
   * @brief Virtual destructor.
   */
  virtual ~AnimState() = default;

  /**
   * @brief Called when the state becomes active. Resets timers and frame indices.
   */
  virtual void OnEnter();

  /**
   * @brief Advances the animation timer and updates the current frame index.
   * @param deltaTime Time elapsed since the last frame.
   */
  virtual void Update(float deltaTime);

  /**
   * @brief Retrieves the texture coordinates of the current animation frame.
   * @return Reference to the SFML IntRect.
   */
  const sf::IntRect& GetCurrentRect() const;

  /**
   * @brief Provides access to the texture associated with this state.
   * @return Constant pointer to the sf::Texture.
   */
  const sf::Texture* GetTexture() const;

  /**
   * @brief Adjusts the playback speed.
   * @param multiplier Speed factor (e.g., 2.0f for double speed).
   */
  void SetSpeed(float multiplier);

  /**
   * @brief Gets the current playback speed multiplier.
   * @return The speed multiplier value.
   */
  float GetSpeed() const;

  /**
   * @brief Checks if a non-looping animation has reached its final frame.
   * @return True if the animation is complete, false otherwise.
   */
  bool IsFinished() const;

 protected:
  Animation m_animation;         ///< Sequence of frames and their durations.
  const sf::Texture* m_texture;  ///< Source atlas for this state's frames.

  float m_elapsedTime = 0.0f;      ///< Timer for the current frame progression.
  float m_speedMultiplier = 1.0f;  ///< Playback rate modifier.
  size_t m_currentFrameIndex = 0;  ///< Index of the currently active frame.

  bool m_isLooping = true;    ///< Flag to enable automatic restart.
  bool m_isFinished = false;  ///< Playback completion status for non-looping states.
};

}  // namespace FalkonEngine