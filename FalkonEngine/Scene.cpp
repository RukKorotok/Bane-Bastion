#include "pch.h"
#include "Scene.h"

namespace FalkonEngine
{
	//Scene
	//-----------------------------------------------------------------------------------------------------------
	Scene::Scene()
	{
		s_activeScene = this;
		m_world = new GameWorld();
	}
	//-----------------------------------------------------------------------------------------------------------
	Scene::~Scene() 
	{
		delete m_world;
		if (s_activeScene == this)
		{
			s_activeScene = nullptr;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	Scene* Scene::GetActive() 
	{ 
		return s_activeScene;
	}
	//-----------------------------------------------------------------------------------------------------------
	GameWorld* Scene::GetWorld() const
	{
		return m_world;
	}
}