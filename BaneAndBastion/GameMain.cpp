
#include <SFML/Graphics.hpp>

#include "Engine.h"
#include "Matrix2D.h"
#include "Player.h"
#include "ResourceSystem.h"
#include "SceneManager.h"

using namespace BaneAndBastion;

int main() {
  sf::RenderWindow window(sf::VideoMode(1280, 720), "Bane & Bastion");
  FalkonEngine::RenderSystem::Instance()->SetMainWindow(&window);

  auto& sm = FalkonEngine::SceneManager::Instance();

  sm.RegisterScene("TestLevel", []() {
    return std::make_unique<BaneAndBastion::GameScene>("TestLevel");
  });

  sm.ApplyLoadScene("TestLevel");

  FE_CORE_INFO("Bane & Bastion: Engine is ready to run.");

  try {
    FalkonEngine::Engine::Instance()->Run();
  } catch (const std::exception& e) {
    FE_CORE_ERROR("CRITICAL ENGINE ERROR: " + std::string(e.what()));
    return -1;
  }

  FE_CORE_INFO("Main: Initiating graceful shutdown...");

  SceneManager::Instance().Clear();

  FE_CORE_INFO("Main: Shutdown complete. Exiting.");

  return 0;
}
