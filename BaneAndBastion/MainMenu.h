#pragma once

#include <array>
#include <iostream>

#include "EnvironmentObject.h"
#include "GridManager.h"
#include "MusicPlayer.h"
#include "Scene.h"

using namespace FalkonEngine;

namespace BaneAndBastion {

/**
 * @brief Primary menu scene for Bane & Bastion.
 * Handles the initial UI layout, menu logic, and transitions to game levels.
 */
class MainMenu : public Scene {
 public:
  /**
   * @brief Constructs the MainMenu scene.
   * @param name The unique identifier for this scene.
   */
  MainMenu(const std::string& name);

  /**
   * @brief Destructor for MainMenu. Handles local resource cleanup.
   */
  ~MainMenu();

  /** @brief LIFECYCLE: Called when the scene is first loaded into memory. */
  void Start() override;

  /** @brief LIFECYCLE: Resets the scene state and UI elements to default. */
  void Restart() override;

  /** @brief LIFECYCLE: Called before the scene is unloaded or switched. */
  void Stop() override;

  /**
   * @brief NOTIFICATION: Central hub for handling engine and UI events within this scene.
   * @param event The dispatched GameEvent to be processed.
   */
  void OnNotify(const GameEvent& event) override;

 private:
  // Scene-specific menu data and UI components will be defined here
};

}  // namespace BaneAndBastion