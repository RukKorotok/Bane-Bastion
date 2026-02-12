
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "GameScene.h"
#include "Matrix2D.h"

using namespace BaneAndBastion;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Bane & Bastion");
	FalkonEngine::RenderSystem::Instance()->SetMainWindow(&window);

	auto* resources = FalkonEngine::ResourceSystem::Instance();
	resources->LoadTexture("monster", "Resources/Textures/monster.png");
	resources->LoadTexture("wall", "Resources/Textures/wall.png");
	resources->LoadTexture("knight", "Resources/Textures/knight.png");

	if (!resources->LoadMusic("NeverSurrender", "Resources/Music/NeverSurrender.wav"))
	{
		FE_CORE_WARN("Main: Initial music not loaded, but continuing...");
	}

	auto developerLevel = std::make_shared<GameScene>("TestLevel");
	developerLevel->Start();

	FE_CORE_INFO("Bane & Bastion: Engine is ready to run.");

	try
	{
		FalkonEngine::Engine::Instance()->Run();
	}
	catch (const std::exception& e)
	{
		FE_CORE_ERROR("CRITICAL ENGINE ERROR: " + std::string(e.what()));
		return -1;
	}

	return 0;
}
