
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Engine.h"
#include "ResourceSystem.h"
#include "GameScene.h"
#include "Matrix2D.h"

using namespace BaneAndBastion;

int main()
{
	FalkonEngine::RenderSystem::Instance()->SetMainWindow(new sf::RenderWindow(sf::VideoMode(1280, 720), "Bane&Bastion"));

	FalkonEngine::ResourceSystem::Instance()->LoadTexture("monster", "Resources/Textures/monster.png");
	FalkonEngine::ResourceSystem::Instance()->LoadTexture("wall", "Resources/Textures/wall.png");
	FalkonEngine::ResourceSystem::Instance()->LoadTexture("knight", "Resources/Textures/knight.png");

	FalkonEngine::ResourceSystem::Instance()->LoadMusic("NeverSurrender", "Resources/Music/NeverSurrender.wav");

	auto developerLevel = std::make_shared<GameScene>();
	developerLevel->Start();

	FalkonEngine::Matrix2D zeroMatrix;
	zeroMatrix.Print();

	FalkonEngine::Matrix2D translationMatrix = FalkonEngine::Matrix2D(Vector2D(12.0f, 5.0f), 0.0f, Vector2Df(1.0f, 1.0f));
	translationMatrix.Print();

	FalkonEngine::Matrix2D rotationMatrix = FalkonEngine::Matrix2D(Vector2D(0.0f, 0.0f), 90.0f, Vector2Df(1.0f, 1.0f));
	rotationMatrix.Print();

	(rotationMatrix * translationMatrix).Print();

	FalkonEngine::Matrix2D someMatrix = FalkonEngine::Matrix2D(Vector2D(13.0f, 25.0f), 90.0f, Vector2Df(1.5f, 1.0f));
	someMatrix.Print();

	(someMatrix * someMatrix.GetInversed()).Print();

	FalkonEngine::Engine::Instance()->Run();

	return 0;
}
