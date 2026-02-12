#include "pch.h"
#include "GameWorld.h"
#include "SpriteColliderComponent.h"
#include "Scene.h"

namespace FalkonEngine
{
	//-----------------------------------------------------------------------------------------------------------
	GameWorld::~GameWorld()
	{
		FE_APP_TRACE("GameWorld destructor called. Clearing all objects.");
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
		float fixedStep = PhysicsSystem::Instance()->GetFixedDeltaTime();

		if (fixedStep <= 0.0001f) {
			FE_CORE_ASSERT(false, "Physics FixedDeltaTime is too small or zero!");
			return;
		}

		if (m_fixedCounter > fixedStep)
		{
			m_fixedCounter -= fixedStep;
			PhysicsSystem::Instance()->Update();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::Render()
	{
		for (size_t i = 0; i < m_gameObjects.size(); i++)
		{
			if (m_gameObjects[i]) 
			{
				m_gameObjects[i]->Render();
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::LateUpdate()
	{
		if (!m_markedToDestroyGameObjects.empty())
		{
			FE_APP_TRACE("LateUpdate: Destroying " + std::to_string(m_markedToDestroyGameObjects.size()) + " marked objects.");

			for (int i = static_cast<int>(m_markedToDestroyGameObjects.size()) - 1; i >= 0; i--)
			{
				DestroyGameObjectImmediate(m_markedToDestroyGameObjects[i]);
			}
			m_markedToDestroyGameObjects.clear();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::DestroyGameObject(GameObject* gameObject)
	{
		if (!gameObject) 
		{
			return;
		}

		auto it = std::find(m_markedToDestroyGameObjects.begin(), m_markedToDestroyGameObjects.end(), gameObject);

		if (it == m_markedToDestroyGameObjects.end())
		{
			m_markedToDestroyGameObjects.push_back(gameObject);
			FE_APP_TRACE("GameObject '" + gameObject->GetName() + "' marked for destruction.");
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameWorld::Clear()
	{
		FE_CORE_INFO("Clearing GameWorld...");

		for (int i = static_cast<int>(m_gameObjects.size()) - 1; i >= 0; i--)
		{
			if (m_gameObjects[i] == nullptr) 
			{
				continue;
			}

			auto transform = m_gameObjects[i]->GetComponent<TransformComponent>();
			if (transform && transform->GetParent() == nullptr)
			{
				DestroyGameObjectImmediate(m_gameObjects[i]);
			}
		}
		m_gameObjects.clear();
		m_markedToDestroyGameObjects.clear();
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

		std::string objName = gameObject->GetName();
		uint32_t objId = gameObject->GetID();

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

		FE_APP_TRACE("Immediate destruction of '" + objName + "' and its " + std::to_string(allTransforms.size() - 1) + " children.");

		for (auto* t : allTransforms) {
			GameObject* obj = t->GetGameObject();
			if (!obj) continue;

			m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), obj), m_gameObjects.end());
			m_markedToDestroyGameObjects.erase(std::remove(m_markedToDestroyGameObjects.begin(), m_markedToDestroyGameObjects.end(), obj), m_markedToDestroyGameObjects.end());

			delete obj;
		}
	}
}