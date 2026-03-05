#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Component.h"
#include "Vector.h"

namespace FalkonEngine {

/**
 * @brief Component that manages and plays spatialized sound effects.
 * Provides features for 3D audio positioning, priority-based playback,
 * and automatic randomization of pitch and volume for natural sound variety.
 */
class SoundComponent : public Component {
 public:
  /**
   * @brief Constructs a SoundComponent and initializes audio settings.
   * @param gameObject Pointer to the owner GameObject.
   */
  SoundComponent(GameObject* gameObject);

  /**
   * @brief Destructor. Stops all active sounds and cleans up resources.
   */
  virtual ~SoundComponent();

  /**
   * @brief Plays a sound from ResourceSystem with 3D positioning and priority.
   * @param name Resource key of the sound buffer in ResourceSystem.
   * @param priority Playback priority. Lower priority sounds won't interrupt higher ones.
   * @param volume Base volume level (0.0f to 100.0f).
   * @param pitch Base pitch multiplier (speed/tone).
   * @param loop Whether the sound should restart automatically after finishing.
   */
  void Play(const std::string& name, int priority = 0, float volume = 100.0f, float pitch = 1.0f, bool loop = false);

  /**
   * @brief Immediately stops all currently playing sounds managed by this component.
   */
  void StopAll();

  /**
   * @brief Synchronizes sound positions with the GameObject's transform and cleans up finished sounds.
   * @param deltaTime Time elapsed since the last frame.
   */
  void Update(float deltaTime) override;

  /**
   * @brief Empty implementation for the Render pass.
   */
  void Render() override {}

 private:
  /**
   * @brief Updates the 3D position of all active SFML sound instances to match the TransformComponent.
   */
  void UpdateSpatialAudio();

  /**
   * @brief Removes sounds from the active list that have finished playing.
   */
  void CleanUpFinishedSounds();

 private:
  std::vector<std::unique_ptr<sf::Sound>> m_activeSounds;  ///< List of currently playing sound instances.

  float m_minDistance = 300.0f;    ///< The distance within which the sound is heard at full volume.
  float m_attenuation = 0.02f;     ///< How much the volume drops as the distance from the listener increases.
  int m_currentMaxPriority = 0;    ///< Highest priority level among currently active sounds.
  float m_pitchVariation = 0.1f;   ///< Range for random pitch adjustment (+/- 10%).
  float m_volumeVariation = 5.0f;  ///< Range for random volume adjustment (+/- 5 units).
};

}  // namespace FalkonEngine