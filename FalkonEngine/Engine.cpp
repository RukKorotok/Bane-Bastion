#include "pch.h"

#include "Engine.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>

#include "ConsolSink.h"
#include "FileSink.h"
#include "LoggerRegistry.h"
#include "RenderSystem.h"
#include "SceneManager.h"

namespace FalkonEngine {
// Engine
//--------------------------------------------------------------------------------------------------------
Engine* Engine::Instance() {
  static Engine instance;
  return &instance;
}

//--------------------------------------------------------------------------------------------------------
Engine::Engine() {
  std::string logDirectory = "Logs";
  std::string logFileName = logDirectory + "/engine_log.txt";

  try {
    if (!std::filesystem::exists(logDirectory)) {
      std::filesystem::create_directories(logDirectory);
    }
  } catch (const std::filesystem::filesystem_error& e) {
    fprintf(stderr, "Could not create Logs directory: %s\n", e.what());
    logFileName = "engine_log.txt";
  }
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

  unsigned int seed = (unsigned int)time(nullptr);
  srand(seed);
  FE_CORE_INFO("Seed initialized: " + std::to_string(seed));
}

//--------------------------------------------------------------------------------------------------------
void Engine::Run() {
  FE_CORE_INFO("Engine::Run() started.");

  FE_CORE_ASSERT(RenderSystem::Instance() != nullptr,
                 "RenderSystem instance is null!");

  sf::RenderWindow& window = RenderSystem::Instance()->GetMainWindow();
  sf::Clock gameClock;
  sf::Event event;

  while (window.isOpen()) {
    sf::Time dt = gameClock.restart();
    float deltaTime = dt.asSeconds();

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        FE_CORE_INFO("Close event received from OS.");
        window.close();
      }
    }

    if (!window.isOpen()) {
      break;
    }

    window.clear();

    SceneManager::Instance().Update(deltaTime);
    SceneManager::Instance().Render();

    window.display();
  }
  FE_CORE_INFO("Engine run loop finished gracefully.");
}
}  // namespace FalkonEngine