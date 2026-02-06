#include "pch.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

namespace FalkonEngine
{
	//CameraComponent
	//-----------------------------------------------------------------------------------------------------------
	CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject)
	{
		m_view = new sf::View(sf::FloatRect(0.0f, 0.0f, 800.0f, -600.0f));
		m_transform = gameObject->GetComponent<TransformComponent>();
	}
	//-----------------------------------------------------------------------------------------------------------
	CameraComponent::~CameraComponent()
	{
		delete m_view;
	}
	//-----------------------------------------------------------------------------------------------------------
	void CameraComponent::Update(float deltaTime)
	{
		auto position = m_transform->GetWorldPosition();
		auto rotation = m_transform->GetWorldRotation();

		m_view->setCenter(Convert<sf::Vector2f, Vector2Df>(position));
		m_view->setRotation(rotation);

		m_window->setView(*m_view);
	}
	//-----------------------------------------------------------------------------------------------------------
	void CameraComponent::Render()
	{
		if (m_window == nullptr)
		{
			std::cout << "NULL window render." << std::endl;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void CameraComponent::SetBaseResolution(int width, int height)
	{
		m_view->reset(sf::FloatRect(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(-height)));
	}
	//-----------------------------------------------------------------------------------------------------------
	void CameraComponent::SetWindow(sf::RenderWindow* newWindow)
	{
		m_window = newWindow;
	}
	//-----------------------------------------------------------------------------------------------------------
	void CameraComponent::ZoomBy(float newZoom)
	{
		if (newZoom <= 0)
		{
			std::cout << "Not allowed zoom lesser or equal than zero." << std::endl;
			return;
		}
		m_view->zoom(newZoom);
	}
}