#pragma once
#include <SFML/Window.hpp>
#include <map>
#include <string>
#include <vector>

#include "SubScriptionSystem.h"  // Исправлено: SubScriptionSystem (согласно твоим прошлым файлам)
#include "Vector.h"

namespace FalkonEngine {

/**
 * @brief Represents a semantic input event triggered by the player.
 */
struct InputAction {
  std::string name;  ///< The ID of the action (e.g., "Horizontal", "Jump")
  float value;       ///< The intensity or direction (e.g., 1.0f or -1.0f)
};

/**
 * @brief Singleton manager for handling player input and mapping it to game actions.
 * Decouples raw hardware keys from gameplay logic via an Action-Binding system.
 */
class PlayerController : public Observable {
 public:
  /** @return The global instance of the PlayerController. */
  PlayerController();
  ~PlayerController() = default;

  /**
   * @brief Maps a keyboard key to a named axis.
   * @param key The SFML key code.
   * @param actionName The string ID for the action.
   * @param scale The multiplier for the axis value (usually 1.0 or -1.0).
   */
  void BindAxis(sf::Keyboard::Key key, const std::string& actionName, float scale);

  /**
   * @brief Maps a mouse button to a named action.
   * @param button The SFML mouse button.
   * @param actionName The string ID for the action.
   */
  void BindMouseButton(sf::Mouse::Button button, const std::string& actionName);

  /**
   * @brief Processes current key states and dispatches continuous axis events.
   * Should be called every frame.
   */
  void Update();

  /**
   * @brief Processes discrete SFML events (like KeyPressed or ButtonReleased).
   * @param event The raw SFML event from the window poll.
   */
  void HandleRawEvent(const sf::Event& event);

  /** * @return The current mouse position converted to world coordinates.
   */
  Vector2Df GetMouseWorldPos() const;

  /** @brief Direct polling for a keyboard key state. */
  bool IsKeyPressed(sf::Keyboard::Key key) const;

  /** @brief Direct polling for a mouse button state. */
  bool IsMouseButtonPressed(sf::Mouse::Button button) const;

 private:

  /** @brief Internal helper to notify observers about mouse actions. */
  void DispatchMouseEvent(sf::Mouse::Button button, bool isPressed);

  // Mappings for discrete and continuous input
  std::map<sf::Keyboard::Key, std::vector<InputAction>> m_bindings;
  std::map<sf::Keyboard::Key, bool> m_keyWasDown;
  std::map<sf::Mouse::Button, std::string> m_mouseBindings;


  Vector2Df m_lastMousePos;  ///< Cached screen position of the mouse.
};

}  // namespace FalkonEngine