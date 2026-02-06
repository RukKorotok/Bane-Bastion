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
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void MusicPlayer::Stop() 
    {
        if (m_currentMusic) 
        {
            m_currentMusic->stop();
            m_currentMusic = nullptr;
            m_currentTrackName = "";
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void MusicPlayer::Pause() 
    {
        if (m_currentMusic) m_currentMusic->pause();
    }
    //--------------------------------------------------------------------------------------------------------
    void MusicPlayer::Resume() 
    {
        if (m_currentMusic && m_currentMusic->getStatus() == sf::Music::Paused) 
        {
            m_currentMusic->play();
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void MusicPlayer::SetVolume(float volume) 
    {
        if (m_currentMusic) m_currentMusic->setVolume(volume);
    }
}