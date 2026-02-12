#include "pch.h"

#include "MusicPlayer.h"
#include "ResourceSystem.h"

namespace FalkonEngine 
{
    //MusicPlayer
    //--------------------------------------------------------------------------------------------------------
    void MusicPlayer::Play(const std::string& name, bool loop, float volume) 
    {
        if (m_currentTrackName == name && m_currentMusic && m_currentMusic->getStatus() == sf::Music::Playing) 
        {
            return;
        }

        Stop();

        m_currentMusic = ResourceSystem::Instance()->GetMusic(name);

        if (m_currentMusic)
        {
            m_currentTrackName = name;
            m_currentMusic->setLoop(loop);
            m_currentMusic->setVolume(volume);
            m_currentMusic->play();

            FE_CORE_INFO("MusicPlayer: Playing track '" + name + "' [Loop: " + (loop ? "Yes" : "No") + ", Vol: " + std::to_string(volume) + "]");
        }
        else
        {
            FE_CORE_ERROR("MusicPlayer: Failed to play track '" + name + "'. Not found in ResourceSystem!");
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void MusicPlayer::Stop() 
    {
        if (m_currentMusic) 
        {
            FE_APP_TRACE("MusicPlayer: Stopping track '" + m_currentTrackName + "'");
            m_currentMusic->stop();
            m_currentMusic = nullptr;
            m_currentTrackName = "";
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void MusicPlayer::Pause() 
    {
        if (m_currentMusic) m_currentMusic->pause();
        FE_APP_TRACE("MusicPlayer: Paused '" + m_currentTrackName + "'");
    }
    //--------------------------------------------------------------------------------------------------------
    void MusicPlayer::Resume() 
    {
        if (m_currentMusic && m_currentMusic->getStatus() == sf::Music::Paused) 
        {
            m_currentMusic->play();
            FE_APP_TRACE("MusicPlayer: Resumed '" + m_currentTrackName + "'");
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void MusicPlayer::SetVolume(float volume) 
    {
        float clampedVolume = (volume < 0.f) ? 0.f : (volume > 100.f ? 100.f : volume);

        if (m_currentMusic)
        {
            m_currentMusic->setVolume(clampedVolume);
        }
    }
}