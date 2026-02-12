#include "pch.h"
#include "GameObject.h"

namespace FalkonEngine
{
	//GameObject
	//-----------------------------------------------------------------------------------------------------------
	GameObject::GameObject() : m_name("GameObject"), m_id(s_nextID++)
	{
		AddComponent<TransformComponent>();
		FE_APP_TRACE("GameObject created: [Default Name] ID: " + std::to_string(m_id));
	}
	//-----------------------------------------------------------------------------------------------------------
	GameObject::GameObject(std::string newName) : m_name(newName), m_id(s_nextID++)
	{
		AddComponent<TransformComponent>();
		FE_APP_TRACE("GameObject created: " + m_name + " ID: " + std::to_string(m_id));
	}
	//-----------------------------------------------------------------------------------------------------------
	GameObject::~GameObject()
	{
		FE_APP_TRACE("Destroying GameObject: " + m_name + " (ID: " + std::to_string(m_id) + ")");

		for (auto component : m_components)
		{
			if (component)
			{
				delete component;
			}
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
		if (id >= s_nextID) 
		{
			s_nextID = id + 1;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObject::Print(int depth) const
	{
		std::string indent(depth * 2, ' ');

		FE_CORE_INFO(indent + "GameObject: " + m_name + " (ID: " + std::to_string(m_id) + ")");

		for (auto& component : m_components)
		{
			FE_CORE_INFO(indent + "  ::Component [" + std::to_string(reinterpret_cast<uintptr_t>(component)) + "]");
		}

		for (GameObject* child : m_children)
		{
			if (child) 
			{
				child->Print(depth + 1);
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObject::Update(float deltaTime)
	{
		for (size_t i = 0; i < m_components.size(); ++i)
		{
			try
			{
				if (m_components[i])
				{
					m_components[i]->Update(deltaTime);
				}
			}
			catch (const std::exception& e)
			{
				FE_CORE_ERROR("Exception in Update of Component on '" + m_name + "': " + e.what());
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObject::Render()
	{
		for (auto& component : m_components)
		{
			if (component)
			{
				component->Render();
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObject::AddChild(GameObject* child)
	{
		m_children.push_back(child);

		if (!child)
		{
			FE_CORE_WARN("Attempted to add a null child to GameObject: " + m_name);
			return;
		}

		auto it = std::find(m_children.begin(), m_children.end(), child);
		if (it != m_children.end())
		{
			FE_CORE_WARN("GameObject '" + child->GetName() + "' is already a child of '" + m_name + "'");
			return;
		}

		m_children.push_back(child);
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameObject::RemoveChild(GameObject* child)
	{
		if (!child) 
		{
			return;
		}

		auto it = std::remove(m_children.begin(), m_children.end(), child);

		if (it != m_children.end())
		{
			m_children.erase(it, m_children.end());
		}
		else
		{
			FE_CORE_WARN("Attempted to remove non-existent child '" + child->GetName() + "' from '" + m_name + "'");
		}
	}
}