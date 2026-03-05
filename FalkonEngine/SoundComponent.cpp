#include "pch.h"

#include "ResourceSystem.h"
#include "SoundComponent.h"
#include "TransformComponent.h"

namespace FalkonEngine {

// SoundComponent --- Event-driven spatial audio management with priority logic ---
//-----------------------------------------------------------------------------------------------------------
SoundComponent::SoundComponent(GameObject* gameObject) : Component(gameObject) {
  FE_APP_TRACE("SoundComponent added to: " + p_gameObject->GetName());
}

//-----------------------------------------------------------------------------------------------------------
SoundComponent::~SoundComponent() { StopAll(); }

//-----------------------------------------------------------------------------------------------------------
void SoundComponent::Play(const std::string& name, int priority, float volume, float pitch, bool loop) {
  // PRIORITY CHECK: Ignore the request if a higher priority sound is currently dominating
  if (!m_activeSounds.empty() && priority < m_currentMaxPriority) {
    return;
  }

  // RESOURCE RETRIEVAL: Get shared buffer from the central system
  const sf::SoundBuffer* buffer = ResourceSystem::Instance()->GetSoundShared(name);
  if (!buffer) {
    return;
  }

  // SPAM PROTECTION: Do not start a new instance if the same sound is already playing
  for (const auto& activeSound : m_activeSounds) {
    if (activeSound->getBuffer() == buffer && activeSound->getStatus() == sf::Sound::Playing) {
      return;
    }
  }

  // RANDOMIZATION: Apply jitter to pitch and volume for more natural variation
  float jitteredPitch = pitch + ((rand() % 200 - 100) / 1000.0f * m_pitchVariation * 10.0f);
  float jitteredVolume = volume + (rand() % 10 - 5);

  // MAINTENANCE: Clean up finished buffers before allocating a new one
  CleanUpFinishedSounds();

  if (m_activeSounds.empty()) {
    m_currentMaxPriority = 0;
  }

  // INITIALIZATION: Setup new sound instance with spatial and variation parameters
  auto sound = std::make_unique<sf::Sound>(*buffer);

  sound->setPitch(jitteredPitch);
  sound->setVolume(std::clamp(jitteredVolume, 0.0f, 100.0f));
  sound->setLoop(loop);

  // SPATIAL SETUP: Configure 3D positioning parameters
  sound->setRelativeToListener(false);
  sound->setMinDistance(m_minDistance);
  sound->setAttenuation(m_attenuation);

  // AUDIO TRIGGER: Start playback and register current priority
  sound->play();

  m_currentMaxPriority = std::max(m_currentMaxPriority, priority);
  m_activeSounds.push_back(std::move(sound));
}

//-----------------------------------------------------------------------------------------------------------
void SoundComponent::Update(float deltaTime) {
  // SYNC: Update sound positions to match transform movements
  UpdateSpatialAudio();

  // LIFECYCLE: Periodically clean up sounds that finished playing
  if (!m_activeSounds.empty()) {
    CleanUpFinishedSounds();
  }
}

//-----------------------------------------------------------------------------------------------------------
void SoundComponent::UpdateSpatialAudio() {
  auto transform = p_gameObject->GetComponent<TransformComponent>();
  if (!transform) return;

  // WORLD POSITION: Sync each active sound's coordinates with the owner's transform
  Vector2Df pos = transform->GetWorldPosition();
  for (auto& sound : m_activeSounds) {
    if (sound->getStatus() == sf::Sound::Playing) {
      sound->setPosition(pos.x, pos.y, 0.0f);
    }
  }
}

//-----------------------------------------------------------------------------------------------------------
void SoundComponent::CleanUpFinishedSounds() {
  // ERASE-REMOVE PATTERN: Remove all sounds with 'Stopped' status from the vector
  m_activeSounds.erase(
      std::remove_if(m_activeSounds.begin(), m_activeSounds.end(),
                     [](const std::unique_ptr<sf::Sound>& s) { return s->getStatus() == sf::Sound::Stopped; }),
      m_activeSounds.end());
}

//-----------------------------------------------------------------------------------------------------------
void SoundComponent::StopAll() {
  // CLEARANCE: Forcefully stop all instances and empty the container
  for (auto& s : m_activeSounds) s->stop();
  m_activeSounds.clear();
}

}  // namespace FalkonEngine