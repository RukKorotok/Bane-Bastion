#pragma once
#include "SFML/Audio.hpp"
#include "Observer.h"
namespace Bane_Bastion
{
	class Game;

	enum SoundType
	{
		Music = 0,
		Sound
	};

	enum SoundFile
	{
		HitSound = 0,
		DeathSound,
		IntroSound,
		PauseSound
	};
	class Audio : public GameObserver
	{
	public:

		static Audio& GetInstance()
		{
			static Audio audio;
			return audio;
		}

		std::weak_ptr<GameObserver> GetWeakObserver();
		void InitAudio();
		//Handler
		void DoOnDestracted(std::shared_ptr<GameObservable> observable, ICollidable& other) override;
		void DoOnInteracted(std::shared_ptr<GameObservable> observable) override;

	private:

		Audio() {}
		~Audio() {}

		Audio(const Audio&) = delete;
		Audio& operator = (Audio const&) = delete;

		void PlaySound(std::pair<sf::Sound, SoundType>& soundPair);

		std::pair<sf::Sound, SoundType> m_hit;
		std::pair<sf::Sound, SoundType> m_death;
		std::pair<sf::Sound, SoundType> m_intro;
		std::pair<sf::Sound, SoundType> m_pause;
		std::pair<sf::Sound, SoundType> m_bonus;
		std::pair<sf::Sound, SoundType> m_crash;

		sf::SoundBuffer m_hitWave;
		sf::SoundBuffer m_deathWave;
		sf::SoundBuffer m_introWave;
		sf::SoundBuffer m_pauseWave;
		sf::SoundBuffer m_bonusWave;
		sf::SoundBuffer m_crashWave;
	};
}
