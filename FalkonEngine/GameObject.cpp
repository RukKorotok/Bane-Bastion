#include "pch.h"
#include "GameObject.h"

namespace FalkonEngine
{
	//GameObject
	//-----------------------------------------------------------------------------------------------------------
	GameObject::GameObject()
	{
		m_name = "GameObject";
		AddComponent<TransformComponent>();
		m_id = s_nextID++;
	}
	//-----------------------------------------------------------------------------------------------------------
	GameObject::GameObject(std::string newName)
	{
		m_name = newName;
		AddComponent<TransformComponent>();
		m_id = s_nextID++;
	}
	//-----------------------------------------------------------------------------------------------------------
	GameObject::~GameObject()
	{
		for (auto component : m_components)
		{
			delete component;
		}
		m_components.clear();
		m_children.clear();
	}
	//-----------------------------------------------------------------------------------------------------------
	std::string GameObject::GetName() const
	{
		return m_name;
	}
	//-----------------------------------------------------------------------------------------------------------
	uint32_t GameObject::GetID() const
	{
		return m_id;
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObject::SetID(uint32_t id)
	{
		m_id = id;
		if (id >= s_nextID) {
			s_nextID = id + 1;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObject::Print(int depth) const
	{
		std::cout << std::string(depth * 2, ' ') << GetName() << std::endl;
		for (auto& component : m_components)
		{
			std::cout << std::string(depth * 2, ' ') << "::" << component << std::endl;
		}

		for (GameObject* child : m_children)
		{
			child->Print(depth + 1);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObject::Update(float deltaTime)
	{
		for (auto& component : m_components)
		{
			component->Update(deltaTime);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObject::Render()
	{
		for (auto& component : m_components)
		{
			component->Render();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObject::AddChild(GameObject* child)
	{
		m_children.push_back(child);
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObject::RemoveChild(GameObject* child)
	{
		m_children.erase(std::remove_if(m_children.begin(), m_children.end(), [child](GameObject* obj) { return obj == child; }), m_children.end());
	}
}