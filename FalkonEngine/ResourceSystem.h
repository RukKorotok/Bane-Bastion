#pragma once

#include <map>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace FalkonEngine
{
	class ResourceSystem
	{
	public:
		static ResourceSystem* Instance();

		/*Texture subsystem */
		void LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth = true);
		const sf::Texture* GetTextureShared(const std::string& name) const;
		sf::Texture* GetTextureCopy(const std::string& name) const;
		void DeleteSharedTexture(const std::string& name);

		void LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth = true);
		const sf::Texture* GetTextureMapElementShared(const std::string& name, int elementIndex) const;
		sf::Texture* GetTextureMapElementCopy(const std::string& name, int elementIndex) const;
		int GetTextureMapElementsCount(const std::string& name) const;
		void DeleteSharedTextureMap(const std::string& name);

		/*SoundEffects subsystem */
		void LoadSound(const std::string& name, const std::string& sourcePath);
		const sf::SoundBuffer* GetSoundShared(const std::string& name) const;
		void DeleteSharedSound(const std::string& name);

		/*Music subsystem */
		sf::Music* LoadMusic(const std::string& name, const std::string& sourcePath);
		sf::Music* GetMusic(const std::string& name) const;
		void DeleteMusic(const std::string& name);

		void Clear();

	private:

		ResourceSystem() {}
		~ResourceSystem() {}

		ResourceSystem(ResourceSystem const&) = delete;
		ResourceSystem& operator= (ResourceSystem const&) = delete;

		void DeleteAllTextures();
		void DeleteAllTextureMaps();

		void DeleteAllSounds();
		void DeleteAllMusic();

		std::map<std::string, sf::Texture*> m_textures;
		std::map<std::string, std::vector<sf::Texture*>> m_textureMaps;

		std::map<std::string, sf::SoundBuffer*> m_soundBuffers;
		std::map<std::string, sf::Music*> m_musicTracks;
	};
}