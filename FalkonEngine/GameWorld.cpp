#include "pch.h"
#include "GameWorld.h"
#include "SpriteColliderComponent.h"
#include "Scene.h"

namespace FalkonEngine
{
	//-----------------------------------------------------------------------------------------------------------
	GameWorld::~GameWorld()
	{
		Clear();
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::Update(float deltaTime)
	{
		for (size_t i = 0; i < m_gameObjects.size(); i++)
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
		for (size_t i = 0; i < m_gameObjects.size(); i++)
		{
			m_gameObjects[i]->Render();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::LateUpdate()
	{
		for (int i = static_cast<int>(m_markedToDestroyGameObjects.size()) - 1; i >= 0; i--)
		{
			DestroyGameObjectImmediate(m_markedToDestroyGameObjects[i]);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::DestroyGameObject(GameObject* gameObject)
	{
		m_markedToDestroyGameObjects.push_back(gameObject);
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::Clear()
	{
		size_t i = m_gameObjects.size();
		while(i > 0)
		{
			i--;
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
		
		if (!gameObject) return;
		
		GameEvent confirmEv;
		confirmEv.type = FalkonEngine::GameEventType::ObjectRemoved;
		confirmEv.sender = this;
		confirmEv.object = gameObject;
		this->Notify(confirmEv);

		auto transform = gameObject->GetComponent<TransformComponent>();
		if (transform && transform->GetParent()) {
			transform->GetParent()->GetGameObject()->RemoveChild(gameObject);
		}

		std::vector<TransformComponent*> allTransforms = gameObject->GetComponentsInChildren<TransformComponent>();

		for (auto* t : allTransforms) {
			GameObject* obj = t->GetGameObject();
			if (!obj) continue;

			m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), obj), m_gameObjects.end());
			m_markedToDestroyGameObjects.erase(std::remove(m_markedToDestroyGameObjects.begin(), m_markedToDestroyGameObjects.end(), obj), m_markedToDestroyGameObjects.end());

			delete obj;
		}
	}
}