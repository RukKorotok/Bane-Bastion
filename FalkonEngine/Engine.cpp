#include "pch.h"
#include "Engine.h"
#include <iostream>
#include "Scene.h"
#include "RenderSystem.h"

namespace FalkonEngine
{
	Engine* Engine::Instance()
	{
		static Engine instance;
		return &instance;
	}

	Engine::Engine()
	{
		unsigned int seed = (unsigned int)time(nullptr);
		srand(seed);
	}

	void Engine::Run()
	{
		sf::Clock gameClock;
		sf::Event event;
		
		while (RenderSystem::Instance()->GetMainWindow().isOpen())
		{
			sf::Time dt = gameClock.restart();
			float deltaTime = dt.asSeconds();

			while (RenderSystem::Instance()->GetMainWindow().pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					RenderSystem::Instance()->GetMainWindow().close();
				}
			}

			if (!RenderSystem::Instance()->GetMainWindow().isOpen())
			{
				break;
			}

			RenderSystem::Instance()->GetMainWindow().clear();

			Scene* activeScene = Scene::GetActive();

			if (activeScene)
			{
				GameWorld* world = activeScene->GetWorld();

				world->Update(deltaTime);
				world->FixedUpdate(deltaTime);
				world->Render();
				world->LateUpdate();
			}
			else
			{
				std::cout << "Scene not loaded " << this << std::endl;
			}

			RenderSystem::Instance()->GetMainWindow().display();
		}
	}
}