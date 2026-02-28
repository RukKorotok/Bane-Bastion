#pragma once

#include <SFML/Audio.hpp>
#include <string>

namespace FalkonEngine {

/**
 * @brief Singleton controller for background music playback.
 * Manages the current active music stream, providing high-level controls
 * like Play, Pause, and Volume adjustment.
 */
class MusicPlayer {
 public:
  /** @return The global instance of the MusicPlayer. */
  static MusicPlayer& Instance() {
    static MusicPlayer instance;
    return instance;
  }

  /**
   * @brief Starts playback of a music track.
   * If another track is already playing, it will be stopped first.
   * @param name The registered name of the music resource in ResourceSystem.
   * @param loop Whether the track should restart automatically upon finishing.
   * @param volume Playback volume (0 to 100).
   */
  void Play(const std::string& name, bool loop = true, float volume = 50.f);

  /** @brief Stops playback and resets the current track position. */
  void Stop();

  /** @brief Temporarily halts playback without resetting position. */
  void Pause();

  /** @brief Continues playback from a paused state. */
  void Resume();

  /**
   * @brief Updates the volume of the currently playing track.
   * @param volume New volume level (0 to 100).
   */
  void SetVolume(float volume);

  /** @return The name of the track currently loaded in the player. */
  std::string GetCurrentTrackName() const { return m_currentTrackName; }

 private:
  MusicPlayer() = default;
  ~MusicPlayer() = default;

  // Singleton: Prevent copying
  MusicPlayer(const MusicPlayer&) = delete;
  MusicPlayer& operator=(const MusicPlayer&) = delete;

  sf::Music* m_currentMusic = nullptr;  ///< Pointer to the active stream from ResourceSystem.
  std::string m_currentTrackName = "";  ///< Name of the active track for tracking state.
};

}  // namespace FalkonEngine