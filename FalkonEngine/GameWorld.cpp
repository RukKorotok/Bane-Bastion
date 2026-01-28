#include "pch.h"
#include "GameWorld.h"

namespace FalkonEngine
{
	//GameWorld
	//-----------------------------------------------------------------------------------------------------------
	GameWorld* GameWorld::Instance()
	{
		static GameWorld world;
		return &world;
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::Update(float deltaTime)
	{
		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			m_gameObjects[i]->Update(deltaTime);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::FixedUpdate(float deltaTime)
	{
		m_fixedCounter += deltaTime;
		if (m_fixedCounter > PhysicsSystem::Instance()->GetFixedDeltaTime())
		{
			m_fixedCounter -= PhysicsSystem::Instance()->GetFixedDeltaTime();
			PhysicsSystem::Instance()->Update();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::Render()
	{
		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			m_gameObjects[i]->Render();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::LateUpdate()
	{
		for (int i = m_markedToDestroyGameObjects.size() - 1; i >= 0; i--)
		{
			DestroyGameObjectImmediate(m_markedToDestroyGameObjects[i]);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	//GameObject* GameWorld::CreateGameObject()
	//{
	//	GameObject* newGameObject = new GameObject();
	//	m_gameObjects.push_back(newGameObject);
	//	return newGameObject;
	//}
	////-----------------------------------------------------------------------------------------------------------
	//GameObject* GameWorld::CreateGameObject(std::string name)
	//{
	//	GameObject* newGameObject = new GameObject(name);
	//	m_gameObjects.push_back(newGameObject);
	//	return newGameObject;
	//}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::DestroyGameObject(GameObject* gameObject)
	{
		m_markedToDestroyGameObjects.push_back(gameObject);
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::Clear()
	{
		for (int i = m_gameObjects.size() - 1; i >= 0; i--)
		{
			if (m_gameObjects[i] == nullptr)
			{
				continue;
			}

			if (m_gameObjects[i]->GetComponent<TransformComponent>()->GetParent() == nullptr)
			{
				DestroyGameObjectImmediate(m_gameObjects[i]);
			}
		}
		m_fixedCounter = 0.0f;
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::Print() const
	{
		for (auto& obj : m_gameObjects)
		{
			if (obj == nullptr)
			{
				continue;
			}
			if (obj->GetComponent<TransformComponent>()->GetParent() == nullptr)
			{
				obj->Print();
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::DestroyGameObjectImmediate(GameObject* gameObject)
	{
		auto parent = gameObject->GetComponent<TransformComponent>()->GetParent();
		if (parent != nullptr)
		{
			parent->GetGameObject()->RemoveChild(gameObject);
		}

		for (auto transform : gameObject->GetComponentsInChildren<TransformComponent>())
		{
			GameObject* gameObjectToDelete = transform->GetGameObject();

			m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(), [gameObjectToDelete](GameObject* obj) { return obj == gameObjectToDelete; }), m_gameObjects.end());
			m_markedToDestroyGameObjects.erase(std::remove_if(m_markedToDestroyGameObjects.begin(), m_markedToDestroyGameObjects.end(), [gameObjectToDelete](GameObject* obj) { return obj == gameObjectToDelete; }), m_markedToDestroyGameObjects.end());

			delete gameObject;
		}
	}
}