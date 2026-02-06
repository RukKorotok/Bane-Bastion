#pragma once

#include "TransformComponent.h"
#include <iostream>
#include "SubScriptionSystem.h"

namespace FalkonEngine
{
	class TransformComponent;

	class GameObject : public Observer
	{
	public:
		GameObject();
		GameObject(std::string newName);
		~GameObject();

		std::string GetName() const;
		uint32_t GetID() const;
		void SetID(uint32_t id);
		void Print(int depth = 0) const;

		void Update(float deltaTime);
		void Render();

		template <typename T>
		T* AddComponent()
		{
			if constexpr (!std::is_base_of<Component, T>::value)
			{
				std::cout << "T must be derived from Component." << std::endl;
				return nullptr;
			}

			if constexpr (std::is_same<T, TransformComponent>::value)
			{
				if (GetComponent<TransformComponent>() != nullptr)
				{
					std::cout << "Can't add Transform, because it will break the engine loop." << std::endl;
					return nullptr;
				}
			}

			T* newComponent = new T(this);
			m_components.push_back(newComponent);
			newComponent->Subscribe(this);
			std::cout << "Add new component: " << newComponent << std::endl;
			return newComponent;
		}

		void RemoveComponent(Component* component)
		{
			m_components.erase(std::remove_if(m_components.begin(), m_components.end(), [component](Component* obj) { return obj == component; }), m_components.end());
			delete component;
			std::cout << "Deleted component";
		}

		template <typename T>
		T* GetComponent() const
		{
			for (const auto& component : m_components)
			{
				if (auto casted = dynamic_cast<T*>(component))
				{
					return casted;
				}
			}
			return nullptr;
		}

		template <typename T>
		T* GetComponentInChildren() const
		{
			T* component = GetComponent<T>();
			if (component != nullptr || m_children.size() == 0)
			{
				return component;
			}

			for (const auto& child : m_children)
			{
				T* childComponent = child->GetComponentInChildren<T>();
				if (childComponent != nullptr)
				{
					return childComponent;
				}
			}
			return nullptr;
		}

		template <typename T>
		std::vector<T*> GetComponents() const
		{
			std::vector<T*> result;
			for (const auto& component : m_components)
			{
				if (auto casted = dynamic_cast<T*>(component))
				{
					result.push_back(casted);
				}
			}
			return result;
		}

		template <typename T>
		std::vector<T*> GetComponentsInChildren() const
		{
			std::vector<T*> result;
			for (const auto& component : GetComponents<T>())
			{
				result.push_back(component);
			}

			for (const auto& child : m_children)
			{
				for (const auto& childComponent : child->GetComponentsInChildren<T>())
				{
					result.push_back(childComponent);
				}
			}
			return result;
		}

		void OnNotify(const GameEvent& event) override {
			HandleEvent(event);
		}

		friend class GameWorld;
		friend class TransformComponent;

	protected:
		virtual void HandleEvent(const GameEvent& event) = 0;

	private:
		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);

		std::vector<Component*> m_components = {};
		std::vector<GameObject*> m_children = {};
		std::string m_name;
		uint32_t m_id;
		inline static uint32_t s_nextID = 1;
	};
}