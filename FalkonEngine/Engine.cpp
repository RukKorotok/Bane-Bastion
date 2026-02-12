#include "pch.h"
#include "Engine.h"
#include <iostream>
#include "Scene.h"
#include "RenderSystem.h"
#include <stdexcept>
#include "LoggerRegistry.h"
#include "ConsolSink.h"
#include "FileSink.h"
#include <filesystem>

namespace FalkonEngine
{
	Engine* Engine::Instance()
	{
		static Engine instance;
		return &instance;
	}

	Engine::Engine()
	{
		std::string logDirectory = "Logs";
		std::string logFileName = logDirectory + "/engine_log.txt";

		try {
			if (!std::filesystem::exists(logDirectory)) {
				std::filesystem::create_directories(logDirectory);
			}
		}
		catch (const std::filesystem::filesystem_error& e) {
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

	void Engine::Run()
	{
		FE_CORE_INFO("Engine::Run() started.");

		FE_CORE_ASSERT(RenderSystem::Instance() != nullptr, "RenderSystem instance is null!");

		sf::RenderWindow& window = RenderSystem::Instance()->GetMainWindow();
		sf::Clock gameClock;
		sf::Event event;
		
		while (window.isOpen())
		{
			sf::Time dt = gameClock.restart();
			float deltaTime = dt.asSeconds();

			while (RenderSystem::Instance()->GetMainWindow().pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					FE_CORE_INFO("Close event received from OS.");
					window.close();
				}
			}

			if (!window.isOpen())
			{
				break;
			}

			window.clear();

			Scene* activeScene = Scene::GetActive();

			if (activeScene)
			{
				GameWorld* world = activeScene->GetWorld();
				if (world)
				{
					world->Update(deltaTime);
					world->FixedUpdate(deltaTime);
					world->Render();
					world->LateUpdate();
				}
				else
				{
					FE_CORE_WARN("Active scene '" + activeScene->GetName() + "' has no GameWorld!");
				}
			}
			else
			{
				FE_CORE_WARN("No scene is currently active. Nothing to render.");
			}

			RenderSystem::Instance()->GetMainWindow().display();
		}
		FE_CORE_INFO("Engine run loop finished gracefully.");
	}
}