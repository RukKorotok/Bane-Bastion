#include "pch.h"

#include "MusicPlayer.h"
#include "ResourceSystem.h"

namespace FalkonEngine {

// MusicPlayer --- High-level control for background music streams ---
//--------------------------------------------------------------------------------------------------------
void MusicPlayer::Play(const std::string& name, bool loop, float volume) {
  // OPTIMIZATION: If the requested track is already playing, do nothing.
  if (m_currentTrackName == name && m_currentMusic && m_currentMusic->getStatus() == sf::Music::Playing) {
    return;
  }

  // STATE MANAGEMENT: Stop any currently playing track before starting a new one.
  Stop();

  // RESOURCE ACQUISITION: Music is streamed, so we get a pointer to the stream object.
  m_currentMusic = ResourceSystem::Instance()->GetMusic(name);

  if (m_currentMusic) {
    m_currentTrackName = name;
    m_currentMusic->setLoop(loop);
    m_currentMusic->setVolume(volume);
    m_currentMusic->play();

    FE_CORE_INFO("MusicPlayer: Playing track '" + name + "' [Loop: " + (loop ? "Yes" : "No") +
                 ", Vol: " + std::to_string(volume) + "]");
  } else {
    FE_CORE_ERROR("MusicPlayer: Failed to play track '" + name + "'. Not found in ResourceSystem!");
  }
}

//--------------------------------------------------------------------------------------------------------
void MusicPlayer::Stop() {
  if (m_currentMusic) {
    FE_APP_TRACE("MusicPlayer: Stopping track '" + m_currentTrackName + "'");
    m_currentMusic->stop();
    m_currentMusic = nullptr;
    m_currentTrackName = "";
  }
}

//--------------------------------------------------------------------------------------------------------
void MusicPlayer::Pause() {
  if (m_currentMusic && m_currentMusic->getStatus() == sf::Music::Playing) {
    m_currentMusic->pause();
    FE_APP_TRACE("MusicPlayer: Paused '" + m_currentTrackName + "'");
  }
}

//--------------------------------------------------------------------------------------------------------
void MusicPlayer::Resume() {
  // Only resume if the track was actually paused to avoid restarting finished tracks.
  if (m_currentMusic && m_currentMusic->getStatus() == sf::Music::Paused) {
    m_currentMusic->play();
    FE_APP_TRACE("MusicPlayer: Resumed '" + m_currentTrackName + "'");
  }
}

//--------------------------------------------------------------------------------------------------------
void MusicPlayer::SetVolume(float volume) {
  // CLAMPING: Ensure volume is always between 0 and 100 to prevent SFML errors.
  float clampedVolume = (volume < 0.f) ? 0.f : (volume > 100.f ? 100.f : volume);

  if (m_currentMusic) {
    m_currentMusic->setVolume(clampedVolume);
  }
}

}  // namespace FalkonEngine