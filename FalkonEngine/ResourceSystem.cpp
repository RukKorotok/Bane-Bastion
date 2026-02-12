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
			FE_CORE_INFO("ResourceSystem: Texture loaded [" + name + "] from " + sourcePath);
		}
		else
		{
			FE_CORE_ERROR("ResourceSystem: Failed to load texture from " + sourcePath);
			delete newTexture;
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
			FE_CORE_WARN("ResourceSystem: Shared texture not found: " + name);
			return nullptr;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	sf::Texture* ResourceSystem::GetTextureCopy(const std::string& name) const
	{
		auto it = m_textures.find(name);
		if (it != m_textures.end())
		{
			return new sf::Texture(*it->second);
		}
		FE_CORE_ERROR("ResourceSystem: Cannot copy texture, not found: " + name);
		return nullptr;
	}
	//-----------------------------------------------------------------------------------------------------------
	void ResourceSystem::DeleteSharedTexture(const std::string& name)
	{
		auto it = m_textures.find(name);

		if (it != m_textures.end())
		{
			delete it->second;
			m_textures.erase(it);
			FE_APP_TRACE("ResourceSystem: Texture deleted: " + name);
		}
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
			std::vector<sf::Texture*> textureMapElements;

			auto textureSize = textureMap.getSize();
			int loadedElements = 0;

			for (unsigned int y = 0; y <= textureSize.y - elementPixelSize.y; y += elementPixelSize.y)
			{
				if (loadedElements == totalElements)
				{
					break;
				}

				for (unsigned int x = 0; x <= textureSize.x - elementPixelSize.x; x += elementPixelSize.x)
				{
					if (loadedElements == totalElements)
					{
						break;
					}

					sf::Texture* newTextureMapElement = new sf::Texture();
					if (newTextureMapElement->loadFromFile(sourcePath, sf::IntRect(x, y, elementPixelSize.x, elementPixelSize.y)))
					{
						newTextureMapElement->setSmooth(isSmooth);
						textureMapElements.push_back(newTextureMapElement);
					}
					else
					{
						delete newTextureMapElement;
					}
					loadedElements++;
				}
			}

			m_textureMaps.emplace(name, textureMapElements);




			if (m_textureMaps.find(name) != m_textureMaps.end()) 
			{
				return;
			}

			sf::Image sourceImage;
			if (sourceImage.loadFromFile(sourcePath))
			{
				std::vector<sf::Texture*> textureMapElements;
				auto imageSize = sourceImage.getSize();
				int loadedElements = 0;

				if (elementPixelSize.x == 0 || elementPixelSize.y == 0) {
					FE_CORE_ERROR("ResourceSystem: Invalid element size for TextureMap " + name);
					return;
				}

				for (unsigned int y = 0; y <= imageSize.y - elementPixelSize.y && loadedElements < totalElements; y += elementPixelSize.y)
				{
					for (unsigned int x = 0; x <= imageSize.x - elementPixelSize.x && loadedElements < totalElements; x += elementPixelSize.x)
					{
						sf::Texture* newElement = new sf::Texture();

						if (newElement->loadFromImage(sourceImage, sf::IntRect(x, y, elementPixelSize.x, elementPixelSize.y)))
						{
							newElement->setSmooth(isSmooth);
							textureMapElements.push_back(newElement);
						}
						else
						{
							delete newElement;
						}
						loadedElements++;
					}
				}
				m_textureMaps.emplace(name, textureMapElements);
				FE_CORE_INFO("ResourceSystem: TextureMap loaded [" + name + "]. Elements: " + std::to_string(textureMapElements.size()));
			}
			else
			{
				FE_CORE_ERROR("ResourceSystem: Failed to load TextureMap from " + sourcePath);
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	const sf::Texture* ResourceSystem::GetTextureMapElementShared(const std::string& name, int elementIndex) const
	{
		auto it = m_textureMaps.find(name);
		if (it == m_textureMaps.end()) 
		{
			FE_CORE_ERROR("ResourceSystem: TextureMap not found: " + name);
			return nullptr;
		}

		if (elementIndex < 0 || elementIndex >= it->second.size()) 
		{
			FE_CORE_WARN("ResourceSystem: Index out of bounds in TextureMap " + name);
			return nullptr;
		}

		return it->second[elementIndex];
	}
	//-----------------------------------------------------------------------------------------------------------
	sf::Texture* ResourceSystem::GetTextureMapElementCopy(const std::string& name, int elementIndex) const
	{
		auto it = m_textureMaps.find(name);
		if (it == m_textureMaps.end())
		{
			FE_CORE_ERROR("ResourceSystem: Cannot copy element. TextureMap not found: " + name);
			return nullptr;
		}

		const std::vector<sf::Texture*>& textures = it->second;
		if (elementIndex < 0 || elementIndex >= static_cast<int>(textures.size()))
		{
			FE_CORE_ERROR("ResourceSystem: Index out of bounds in TextureMap '" + name +
				"'. Index: " + std::to_string(elementIndex) +
				", Size: " + std::to_string(textures.size()));
			return nullptr;
		}

		if (textures[elementIndex] == nullptr)
		{
			FE_CORE_ERROR("ResourceSystem: Texture at index " + std::to_string(elementIndex) +
				" in Map '" + name + "' is null!");
			return nullptr;
		}

		FE_APP_TRACE("ResourceSystem: Created copy of TextureMap element [" + name + ":" + std::to_string(elementIndex) + "]");
		return new sf::Texture(*textures[elementIndex]);
	}
	//-----------------------------------------------------------------------------------------------------------
	int ResourceSystem::GetTextureMapElementsCount(const std::string& name) const
	{
		auto it = m_textureMaps.find(name);
		if (it == m_textureMaps.end())
		{
			return 0;
		}

		return static_cast<int>(it->second.size());
	}
	//-----------------------------------------------------------------------------------------------------------
	void ResourceSystem::DeleteSharedTextureMap(const std::string& name)
	{
		auto textureMap = m_textureMaps.find(name);
		if (textureMap != m_textureMaps.end())
		{
			for (sf::Texture* deletingTexture : textureMap->second)
			{
				delete deletingTexture;
			}

			m_textureMaps.erase(textureMap);
		}
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
			FE_CORE_INFO("ResourceSystem: Sound loaded [" + name + "]");
		}
		else
		{
			FE_CORE_ERROR("ResourceSystem: Failed to load sound: " + sourcePath);
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
			FE_CORE_INFO("ResourceSystem: Music opened [" + name + "]");
			return music;
		}
		else
		{
			FE_CORE_ERROR("ResourceSystem: Failed to open music: " + sourcePath);
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
		FE_APP_TRACE("ResourceSystem: Clearing all resources...");
		DeleteAllTextures();
		DeleteAllTextureMaps();
		DeleteAllSounds();
		DeleteAllMusic();
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