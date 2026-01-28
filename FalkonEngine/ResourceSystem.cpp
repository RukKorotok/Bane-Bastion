#include "pch.h"
#include "ResourceSystem.h"

namespace FalkonEngine
{
	//ResourceSystem
	//-----------------------------------------------------------------------------------------------------------
	ResourceSystem* ResourceSystem::Instance()
	{
		static ResourceSystem resourceSystem;
		return &resourceSystem;
	}
	//Texture subsystem
	//-----------------------------------------------------------------------------------------------------------
	void ResourceSystem::LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth)
	{
		if (m_textures.find(name) != m_textures.end())
		{
			return;
		}

		sf::Texture* newTexture = new sf::Texture();
		if (newTexture->loadFromFile(sourcePath))
		{
			newTexture->setSmooth(isSmooth);
			m_textures.emplace(name, newTexture);
			std::cout << "[ResourceSystem] Texture loaded: " << name << std::endl;
		}
		else
		{
			std::cerr << "[ResourceSystem] Failed to load texture : " << sourcePath + name << std::endl;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	const sf::Texture* ResourceSystem::GetTextureShared(const std::string& name) const
	{
		auto it = m_textures.find(name);
		if (it != m_textures.end())
		{
			return it->second;
		}
		else
		{
			return nullptr;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	sf::Texture* ResourceSystem::GetTextureCopy(const std::string& name) const
	{
		return new sf::Texture(*m_textures.find(name)->second);
	}
	//-----------------------------------------------------------------------------------------------------------
	void ResourceSystem::DeleteSharedTexture(const std::string& name)
	{
		auto texturePair = m_textures.find(name);

		sf::Texture* deletingTexure = texturePair->second;
		m_textures.erase(texturePair);
		delete deletingTexure;
	}
	//-----------------------------------------------------------------------------------------------------------
	void ResourceSystem::LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth)
	{
		if (m_textureMaps.find(name) != m_textureMaps.end())
		{
			return;
		}

		sf::Texture textureMap;
		if (textureMap.loadFromFile(sourcePath))
		{
			auto textureMapElements = new std::vector<sf::Texture*>();

			auto textureSize = textureMap.getSize();
			int loadedElements = 0;

			for (int y = 0; y <= textureSize.y - elementPixelSize.y; y += elementPixelSize.y)
			{
				if (loadedElements == totalElements)
				{
					break;
				}

				for (int x = 0; x <= textureSize.x - elementPixelSize.x; x += elementPixelSize.x)
				{
					if (loadedElements == totalElements)
					{
						break;
					}

					sf::Texture* newTextureMapElement = new sf::Texture();
					if (newTextureMapElement->loadFromFile(sourcePath, sf::IntRect(x, y, elementPixelSize.x, elementPixelSize.y)))
					{
						newTextureMapElement->setSmooth(isSmooth);
						textureMapElements->push_back(newTextureMapElement);
					}
					loadedElements++;
				}
			}

			m_textureMaps.emplace(name, *textureMapElements);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	const sf::Texture* ResourceSystem::GetTextureMapElementShared(const std::string& name, int elementIndex) const
	{
		auto textureMap = m_textureMaps.find(name);
		auto textures = textureMap->second;
		return textures[elementIndex];
	}
	//-----------------------------------------------------------------------------------------------------------
	sf::Texture* ResourceSystem::GetTextureMapElementCopy(const std::string& name, int elementIndex) const
	{
		auto textureMap = m_textureMaps.find(name);
		auto textures = textureMap->second;
		return new sf::Texture(*textures[elementIndex]);
	}
	//-----------------------------------------------------------------------------------------------------------
	int ResourceSystem::GetTextureMapElementsCount(const std::string& name) const
	{
		auto textureMap = m_textureMaps.find(name);
		auto textures = textureMap->second;
		return textures.size();
	}
	//-----------------------------------------------------------------------------------------------------------
	void ResourceSystem::DeleteSharedTextureMap(const std::string& name)
	{
		auto textureMap = m_textureMaps.find(name);
		auto deletingTextures = textureMap->second;

		for (int i = 0; i < deletingTextures.size(); i++)
		{
			delete deletingTextures[i];
		}

		m_textureMaps.erase(textureMap);
	}
	//SundEffects system
	//-----------------------------------------------------------------------------------------------------------
	void ResourceSystem::LoadSound(const std::string& name, const std::string& sourcePath)
	{
		if (m_soundBuffers.find(name) != m_soundBuffers.end()) return;

		sf::SoundBuffer* buffer = new sf::SoundBuffer();
		if (buffer->loadFromFile(sourcePath))
		{
			m_soundBuffers[name] = buffer;
			std::cout << "[ResourceSystem] Sound loaded: " << name << std::endl;
		}
		else
		{
			std::cerr << "[ResourceSystem] Failed to load sound: " << sourcePath << std::endl;
			delete buffer;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	const sf::SoundBuffer* ResourceSystem::GetSoundShared(const std::string& name) const
	{
		auto it = m_soundBuffers.find(name);
		if (it != m_soundBuffers.end())
		{
			return it->second;
		}
		else
		{
			return nullptr;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void ResourceSystem::DeleteSharedSound(const std::string& name)
	{
		auto it = m_soundBuffers.find(name);
		if (it != m_soundBuffers.end())
		{
			delete it->second;
			m_soundBuffers.erase(it);
		}
	}
	//Music system
	//-----------------------------------------------------------------------------------------------------------
	sf::Music* ResourceSystem::LoadMusic(const std::string& name, const std::string& sourcePath)
	{
		if (m_musicTracks.find(name) != m_musicTracks.end())
		{
			return m_musicTracks[name];
		}

		sf::Music* music = new sf::Music();
		if (music->openFromFile(sourcePath))
		{
			m_musicTracks[name] = music;
			std::cout << "[ResourceSystem] Music opened: " << name << std::endl;
			return music;
		}
		else
		{
			std::cerr << "[ResourceSystem] Failed to open music: " << sourcePath << std::endl;
			delete music;
			return nullptr;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	sf::Music* ResourceSystem::GetMusic(const std::string& name) const
	{
		auto it = m_musicTracks.find(name);
		return (it != m_musicTracks.end()) ? it->second : nullptr;
	}
	//-----------------------------------------------------------------------------------------------------------
	void ResourceSystem::DeleteMusic(const std::string& name)
	{
		auto it = m_musicTracks.find(name);
		if (it != m_musicTracks.end())
		{
			delete it->second;
			m_musicTracks.erase(it);
		}
	}
	//msc
	//-----------------------------------------------------------------------------------------------------------
	void ResourceSystem::Clear()
	{
		DeleteAllTextures();
		DeleteAllTextureMaps();
		DeleteAllSounds();
		DeleteAllMusic();
		std::cout << "[ResourceSystem] All resources cleared." << std::endl;
	}
	//private
	//-----------------------------------------------------------------------------------------------------------
	void ResourceSystem::DeleteAllTextures()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& texturePair : m_textures)
		{
			keysToDelete.push_back(texturePair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSharedTexture(key);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void ResourceSystem::DeleteAllTextureMaps()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& textureMapPair : m_textureMaps)
		{
			keysToDelete.push_back(textureMapPair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSharedTextureMap(key);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void ResourceSystem::DeleteAllSounds()
	{
		for (auto& pair : m_soundBuffers)
		{
			delete pair.second;
		}
		m_soundBuffers.clear();
	}
	//-----------------------------------------------------------------------------------------------------------
	void ResourceSystem::DeleteAllMusic()
	{
		for (auto& pair : m_musicTracks)
		{
			delete pair.second;
		}
		m_musicTracks.clear();
	}
}