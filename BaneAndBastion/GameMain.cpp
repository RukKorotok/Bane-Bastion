#include "pch.h"

#include <SFML/Graphics.hpp>

#include "Engine.h"
#include "MainMenu.h"
#include "Matrix2D.h"
#include "Player.h"
#include "ResourceSystem.h"
#include "SceneManager.h"
#include "StatDefinition.h"
#include "ForestScene.h"
#include "PauseMenu.h"

using namespace BaneAndBastion;

// Main --- Entry point for Bane & Bastion application ---
//-----------------------------------------------------------------------------------------------------------
int main() {
  StatRegistry::RegisterStat("Health", "hp", "hp_max");
  StatRegistry::RegisterStat("Mana", "mp", "mp_max");
  StatRegistry::RegisterStat("Defense", "def", "def_max");
  StatRegistry::RegisterStat("Strength", "str", "str_max");
  StatRegistry::RegisterStat("Hit", "hit", "hit_max");


  // WINDOW: Initialize the primary render surface and link to the engine's render system
  sf::RenderWindow window(sf::VideoMode(1280, 720), "Bane & Bastion");
  FalkonEngine::RenderSystem::Instance()->SetMainWindow(&window);

  // SCENE MANAGEMENT: Register available scenes and set the entry-level scene
  auto& sm = FalkonEngine::SceneManager::Instance();

  sm.RegisterScene("MainMenu", []() { return std::make_unique<MainMenu>("MainMenu"); });
  sm.RegisterScene("Forest", []() { return std::make_unique<ForestScene>("Forest"); });
  sm.RegisterScene("PauseMenu", []() { return std::make_unique<PauseMenu>("PauseMenu"); });

  // BOOTSTRAP: Finalize scene selection before entering the main loop
  sm.ApplyLoadScene("MainMenu");

  FE_CORE_INFO("Bane & Bastion: Engine is ready to run.");

  // EXECUTION: Start the core engine loop with critical error trapping
  try {
    FalkonEngine::Engine::Instance()->Run();
  } catch (const std::exception& e) {
    FE_CORE_ERROR("CRITICAL ENGINE ERROR: " + std::string(e.what()));
    return -1;
  }

  // TERMINATION: Initiate graceful cleanup and resource deallocation
  FE_CORE_INFO("Main: Initiating graceful shutdown...");

  SceneManager::Instance().Clear();

  FE_CORE_INFO("Main: Shutdown complete. Exiting.");

  return 0;
}