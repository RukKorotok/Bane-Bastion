#include "pch.h"
#include "Scene.h"

namespace FalkonEngine
{
	//Scene
	//-----------------------------------------------------------------------------------------------------------
	Scene::Scene(const std::string& name) : m_name(name), m_world(nullptr)
	{
		if (s_activeScene != nullptr)
		{
			FE_CORE_WARN("Creating new scene '" + name + "' while scene '" + s_activeScene->GetName() + "' is still active!");
		}

		s_activeScene = this;

		try
		{
			m_world = new GameWorld();
		}
		catch (const std::exception& e)
		{
			FE_CORE_ERROR("Failed to allocate GameWorld for scene '" + m_name + "': " + e.what());
			throw;
		}

		FE_CORE_INFO("Scene '" + m_name + "' created and set as active.");
	}
	//-----------------------------------------------------------------------------------------------------------
	Scene::~Scene() 
	{
		FE_CORE_INFO("Destroying scene: " + m_name);

		delete m_world;
		if (s_activeScene == this)
		{
			s_activeScene = nullptr;
			FE_APP_TRACE("Active scene pointer cleared (was '" + m_name + "')");
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	Scene* Scene::GetActive() 
	{ 
		if (s_activeScene == nullptr)
		{
			FE_CORE_WARN("Scene::GetActive() called but no scene is currently active!");
		}
		return s_activeScene;
	}
	//-----------------------------------------------------------------------------------------------------------
	GameWorld* Scene::GetWorld() const
	{
		FE_CORE_ASSERT(m_world != nullptr, "GameWorld in scene '" + m_name + "' is null!");

		return m_world;
	}
	std::string Scene::GetName()
	{
		return m_name;
	}
}