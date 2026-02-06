#pragma once

#include <string>
#include <SFML/Audio.hpp>

namespace FalkonEngine 
{
    class MusicPlayer 
    {
    public:
        static MusicPlayer& Instance() 
        {
            static MusicPlayer instance;
            return instance;
        }

        void Play(const std::string& name, bool loop = true, float volume = 50.f);

        void Stop();
        void Pause();
        void Resume();
        void SetVolume(float volume);

        std::string GetCurrentTrackName() const { return m_currentTrackName; }

    private:
        MusicPlayer() = default;
        sf::Music* m_currentMusic = nullptr;
        std::string m_currentTrackName = "";
    };
}