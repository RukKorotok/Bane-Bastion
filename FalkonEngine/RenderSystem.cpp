#include "pch.h"

#include "RenderSystem.h"

namespace FalkonEngine {

// RenderSystem --- Singleton implementation for window management ---
//-----------------------------------------------------------------------------------------------------------
RenderSystem* RenderSystem::Instance() {
  static RenderSystem render;
  return &render;
}

//-----------------------------------------------------------------------------------------------------------
void RenderSystem::SetMainWindow(sf::RenderWindow* newWindow) {
  if (newWindow == nullptr) {
    FE_CORE_ERROR("RenderSystem: Attempted to set a null RenderWindow!");
    return;
  }

  m_window = newWindow;
  FE_CORE_INFO("RenderSystem: Main window successfully attached.");
}

//-----------------------------------------------------------------------------------------------------------
sf::RenderWindow& RenderSystem::GetMainWindow() const {
  // CRITICAL: Ensure the engine doesn't try to render if the window hasn't been created yet.
  FE_CORE_ASSERT(m_window != nullptr, "RenderSystem: Accessing MainWindow before initialization!");

  return *m_window;
}

//-----------------------------------------------------------------------------------------------------------
void RenderSystem::Render(const sf::Drawable& drawable) {
  if (m_window != nullptr) {
    // SFML Dispatch: Send the drawable object (Sprite, Text, etc.) to the GPU.
    m_window->draw(drawable);
  } else {
    // ERROR THROTTLING: Avoid spamming the console 60 times per second if the window is missing.
    static bool windowErrorLogged = false;
    if (!windowErrorLogged) {
      FE_CORE_ERROR("RenderSystem: Cannot draw! MainWindow is null.");
      windowErrorLogged = true;
    }
  }
}

}  // namespace FalkonEngine