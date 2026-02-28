#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace FalkonEngine {

/**
 * @brief Singleton manager for centralized asset handling.
 * Prevents redundant memory allocation by caching textures, sound buffers, and music.
 * Supports individual textures as well as texture maps (atlases).
 */
class ResourceSystem {
 public:
  /** @return The global instance of the ResourceSystem. */
  static ResourceSystem* Instance();

  /* --- Texture Subsystem --- */

  /** @brief Loads a single texture into the shared cache. */
  void LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth = true);

  /** @return Pointer to a cached texture. Do NOT delete this pointer manually. */
  const sf::Texture* GetTextureShared(const std::string& name) const;

  /** @return A new instance of a texture. The caller is responsible for the memory. */
  sf::Texture* GetTextureCopy(const std::string& name) const;

  /** @brief Removes a specific texture from memory. */
  void DeleteSharedTexture(const std::string& name);

  /** @brief Loads an atlas and slices it into individual texture elements. */
  void LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements,
                      bool isSmooth = true);

  const sf::Texture* GetTextureMapElementShared(const std::string& name, int elementIndex) const;
  sf::Texture* GetTextureMapElementCopy(const std::string& name, int elementIndex) const;
  int GetTextureMapElementsCount(const std::string& name) const;
  void DeleteSharedTextureMap(const std::string& name);

  /* --- Sound Effects Subsystem --- */

  /** @brief Loads audio data into a SoundBuffer (best for short effects). */
  void LoadSound(const std::string& name, const std::string& sourcePath);
  const sf::SoundBuffer* GetSoundShared(const std::string& name) const;
  void DeleteSharedSound(const std::string& name);

  /* --- Music Subsystem --- */

  /** @brief Opens a music stream (best for long tracks/ambient). */
  sf::Music* LoadMusic(const std::string& name, const std::string& sourcePath);
  sf::Music* GetMusic(const std::string& name) const;
  void DeleteMusic(const std::string& name);

  /** @brief Purges all loaded resources from memory. Called during scene transitions or shutdown. */
  void Clear();

  /* --- Font Subsystem --- */

  /** @brief Loads a font file into memory. */
  void LoadFont(const std::string& name, const std::string& sourcePath);

  /** @return Pointer to a cached font. Do NOT delete this pointer manually. */
  const sf::Font* GetFontShared(const std::string& name) const;

  /** @brief Removes a specific font from memory. */
  void DeleteSharedFont(const std::string& name);

 private:
  ResourceSystem() {}
  ~ResourceSystem() {}

  // Singleton protection
  ResourceSystem(ResourceSystem const&) = delete;
  ResourceSystem& operator=(ResourceSystem const&) = delete;

  void DeleteAllTextures();
  void DeleteAllTextureMaps();
  void DeleteAllSounds();
  void DeleteAllMusic();
  void DeleteAllFonts();

  std::map<std::string, sf::Texture*> m_textures;                  ///< Single texture cache.
  std::map<std::string, std::vector<sf::Texture*>> m_textureMaps;  ///< Sliced texture atlases.

  std::map<std::string, sf::SoundBuffer*> m_soundBuffers;  ///< Sound effect data.
  std::map<std::string, sf::Music*> m_musicTracks;         ///< Music streams.

  std::map<std::string, sf::Font*> m_fonts; ///< Font resource cache.
};

}  // namespace FalkonEngine