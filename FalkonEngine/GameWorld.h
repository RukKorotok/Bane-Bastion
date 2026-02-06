#pragma once

#include "GameObject.h"
#include "PhysicsSystem.h"

namespace FalkonEngine
{
	class GameWorld : public Observable
	{
	public:
		GameWorld() = default;
		~GameWorld();

		GameWorld(GameWorld const&) = delete;
		GameWorld& operator= (GameWorld const&) = delete;

		void Update(float deltaTime);
		void FixedUpdate(float deltaTime);
		void Render();
		void LateUpdate();

		/*GameObject* CreateGameObject();
		GameObject* CreateGameObject(std::string name);*/
		template<typename T, typename... Args>
		T* CreateGameObject(Args&&... args) 
		{
			static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");

			T* newObject = new T(std::forward<Args>(args)...);
			m_gameObjects.push_back(newObject);
			return newObject;
		}

		void DestroyGameObject(GameObject* gameObject);
		void Clear();

		void Print() const;
	private:

		float m_fixedCounter = 0.f;

		std::vector<GameObject*> m_gameObjects = {};
		std::vector<GameObject*> m_markedToDestroyGameObjects = {};

		void DestroyGameObjectImmediate(GameObject* gameObject);
	};
}