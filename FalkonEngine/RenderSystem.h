#pragma once

#include <SFML/Graphics.hpp>

namespace FalkonEngine {

/**
 * @brief Singleton wrapper for the SFML RenderWindow.
 * Provides a centralized entry point for all drawing operations and
 * window management within the engine.
 */
class RenderSystem {
 public:
  /** @return The global instance of the RenderSystem. */
  static RenderSystem* Instance();

  /**
   * @brief Connects the main SFML window to the system.
   * Usually called once during engine initialization.
   * @param newWindow Pointer to the active RenderWindow instance.
   */
  void SetMainWindow(sf::RenderWindow* newWindow);

  /** * @return Reference to the main window.
   * @throws FE_CORE_ASSERT if the window has not been set yet.
   */
  sf::RenderWindow& GetMainWindow() const;

  /**
   * @brief Draws a single SFML drawable object to the main window.
   * @param drawable Any object inheriting from sf::Drawable (Sprite, Shape, Text, etc.).
   */
  void Render(const sf::Drawable& drawable);

 private:
  sf::RenderWindow* m_window = nullptr;  ///< Pointer to the OS window managed by SFML.

  RenderSystem() {}
  ~RenderSystem() {}

  // Singleton: Prevent copying to ensure only one window context exists.
  RenderSystem(RenderSystem const&) = delete;
  RenderSystem& operator=(RenderSystem const&) = delete;
};

}  // namespace FalkonEngine