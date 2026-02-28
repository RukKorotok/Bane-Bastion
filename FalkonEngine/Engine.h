#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "Log.h"
#include "SFML/Graphics.hpp"

namespace FalkonEngine {

/**
 * @brief The core class of the FalkonEngine.
 * Implements the Singleton pattern to manage the global application state,
 * main game loop, and system subsystems initialization.
 */
class Engine {
 public:
  /**
   * @brief Deleted copy constructor to enforce Singleton pattern.
   */
  Engine(const Engine& app) = delete;

  /**
   * @brief Deleted assignment operator to enforce Singleton pattern.
   */
  Engine& operator=(const Engine&) = delete;

  /**
   * @brief Accesses the global Engine instance.
   * @return A pointer to the static Engine instance.
   */
  static Engine* Instance();

  /**
   * @brief Enters the main execution loop.
   * Handles frame timing, event polling, state updates, and rendering
   * until the main window is closed.
   */
  void Run();

 private:
  /**
   * @brief Private constructor for Singleton initialization.
   * Sets up loggers, random seed, and core engine subsystems.
   */
  Engine();

  /**
   * @brief Default destructor.
   */
  ~Engine() = default;
};

}  // namespace FalkonEngine