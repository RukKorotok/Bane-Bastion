#include "Sound.h"
#include "Game.h"

namespace Bane_Bastion
{
	//------------------------------------------------------------------------------------------------------------
	std::weak_ptr<GameObserver> Audio::GetWeakObserver()
	{
		static std::shared_ptr<GameObserver> proxy(&GetInstance(), [](GameObserver*) {});
		return proxy;
	}
	//------------------------------------------------------------------------------------------------------------
	void Audio::InitAudio()
	{

		assert(m_hitWave.loadFromFile(RESOURCES_PATH + "Hit.wav"));
		assert(m_deathWave.loadFromFile(RESOURCES_PATH + "Death.wav"));
		assert(m_introWave.loadFromFile(RESOURCES_PATH + "Intro.wav"));
		assert(m_pauseWave.loadFromFile(RESOURCES_PATH + "Pause.wav"));
		assert(m_bonusWave.loadFromFile(RESOURCES_PATH + "Bonus.wav"));
		assert(m_crashWave.loadFromFile(RESOURCES_PATH + "Crash.wav"));

		m_hit.first.setBuffer(m_hitWave);
		m_hit.second = SoundType::Sound;
		m_pause.first.setBuffer(m_pauseWave);
		m_pause.second = SoundType::Sound;
		m_death.first.setBuffer(m_deathWave);
		m_death.second = SoundType::Sound;
		m_intro.first.setBuffer(m_introWave);
		m_intro.second = SoundType::Music;
		m_bonus.first.setBuffer(m_bonusWave);
		m_bonus.second = SoundType::Sound;
		m_crash.first.setBuffer(m_crashWave);
		m_crash.second = SoundType::Sound;

	}
	//Handler
	//-----------------------------------------------------------------------------------------------------------
	void Audio::DoOnDestracted(std::shared_ptr<GameObservable> observable, ICollidable& other)
	{
		if (std::dynamic_pointer_cast<SpecialBall>(observable))
		{
			if (std::dynamic_pointer_cast<SpecialBall>(observable) && observable.get() != dynamic_cast<GameObservable*>(&other))
			{
				PlaySound(m_bonus);
			}
		}
		else if (std::dynamic_pointer_cast<MainBall>(observable))
		{
			PlaySound(m_death);
		}
		else if (std::dynamic_pointer_cast<Block>(observable))
		{
			PlaySound(m_crash);
		}

	}
	//-----------------------------------------------------------------------------------------------------------
	void Audio::DoOnInteracted(std::shared_ptr<GameObservable> observable)
	{
		if (std::dynamic_pointer_cast<Ball>(observable))
		{
			PlaySound(m_hit);
		}
	}
	//------------------------------------------------------------------------------------------------------------
	void Audio::PlaySound(std::pair<sf::Sound, SoundType>& soundPair)
	{

		switch (soundPair.second)
		{
		case SoundType::Music:
		{
			if (Game::GetInstance().GetSetings() & static_cast<uint32_t>(SettingsMode::MusicOn))
			{
				soundPair.first.play();
			}
			break;
		}
		case SoundType::Sound:
		{
			if (Game::GetInstance().GetSetings() & static_cast<uint32_t>(SettingsMode::SoundOn))
			{
				soundPair.first.play();
			}
			break;
		}
		}
	}
}
