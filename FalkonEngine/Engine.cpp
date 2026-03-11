#include "pch.h"

#include "Engine.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>

#include "ConsolSink.h"
#include "FileSink.h"
#include "LoggerRegistry.h"
#include "PlayerController.h"
#include "RenderSystem.h"
#include "SceneManager.h"
#include "DeferredActionRegistry.h"

namespace FalkonEngine {

// Engine --- Core Singleton management and main loop execution ---
//--------------------------------------------------------------------------------------------------------
Engine* Engine::Instance() {
  static Engine instance;
  return &instance;
}

//--------------------------------------------------------------------------------------------------------
Engine::Engine() {
  std::string logDirectory = "Logs";
  std::string logFileName = logDirectory + "/engine_log.txt";

  // FILESYSTEM: Initialize logging directory and handle potential access errors
  try {
    if (!std::filesystem::exists(logDirectory)) {
      std::filesystem::create_directories(logDirectory);
    }
  } catch (const std::filesystem::filesystem_error& e) {
    fprintf(stderr, "Could not create Logs directory: %s\n", e.what());
    logFileName = "engine_log.txt";
  }

  // LOGGING: Register sinks for Core and App loggers to handle console and file output
  auto& registry = LoggerRegistry::GetInstance();
  auto consoleSink = std::make_shared<ConsolSink>();
  auto fileSink = std::make_shared<FileSink>(logFileName);

  auto coreLogger = registry.GetLogger("Core");
  coreLogger->AddSink(consoleSink);
  coreLogger->AddSink(fileSink);

  auto appLogger = registry.GetLogger("App");
  appLogger->AddSink(consoleSink);
  appLogger->AddSink(fileSink);

  FE_CORE_INFO("--- FalkonEngine Startup ---");

  // RANDOMIZER: Global seed initialization for procedural logic and physics
  unsigned int seed = (unsigned int)time(nullptr);
  srand(seed);
  FE_CORE_INFO("Seed initialized: " + std::to_string(seed));
}

//--------------------------------------------------------------------------------------------------------
void Engine::Run() {
  FE_CORE_INFO("Engine::Run() started.");

  // SAFETY CHECK: Ensure rendering context is established before entering the loop
  FE_CORE_ASSERT(RenderSystem::Instance() != nullptr, "RenderSystem instance is null!");

  sf::RenderWindow& window = RenderSystem::Instance()->GetMainWindow();
  sf::Clock gameClock;
  sf::Event event;

  // MAIN GAME LOOP: Event handling, state updates, and frame rendering
  while (window.isOpen()) {
    sf::Time dt = gameClock.restart();
    float deltaTime = dt.asSeconds();

    // EVENT PROCESSING: OS events and input redirection
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        FE_CORE_INFO("Close event received from OS.");
        window.close();
      }

      // Input handling via PlayerController
      SceneManager::Instance().GetActiveScene()->GetPlayerController()->HandleRawEvent(event);
    }

    // Final window check before processing graphics
    if (!window.isOpen()) {
      break;
    }

    // FRAME CYCLE: Clear, Update logic, Render objects, and Display result
    window.clear();

    DeferredActionRegistry::Instance().ProcessAll();
    SceneManager::Instance().Update(deltaTime);
    SceneManager::Instance().Render();
    SceneManager::Instance().GetActiveScene()->GetPlayerController()->Update();

    window.display();
  }

  FE_CORE_INFO("Engine run loop finished gracefully.");
}

}  // namespace FalkonEngine