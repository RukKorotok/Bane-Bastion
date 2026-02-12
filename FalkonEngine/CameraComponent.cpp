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

		FE_CORE_ASSERT(m_transform != nullptr, "CameraComponent added to GameObject without TransformComponent!");
	}
	//-----------------------------------------------------------------------------------------------------------
	CameraComponent::~CameraComponent()
	{
		delete m_view;
	}
	//-----------------------------------------------------------------------------------------------------------
	void CameraComponent::Update(float deltaTime)
	{
		if (!m_window)
		{
			FE_CORE_WARN("Window not valid!");
			return;
		}

		FE_CORE_ASSERT(m_transform != nullptr, "Camera Transform lost during update!");

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
			FE_CORE_WARN("Attempted to render CameraComponent with NULL window.");
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void CameraComponent::SetBaseResolution(int width, int height)
	{
		if (width <= 0 || height <= 0)
		{
			FE_CORE_ERROR("Invalid base resolution: " + std::to_string(width) + "x" + std::to_string(height));
			return;
		}
		m_view->reset(sf::FloatRect(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(-height)));
	}
	//-----------------------------------------------------------------------------------------------------------
	void CameraComponent::SetWindow(sf::RenderWindow* newWindow)
	{
		m_window = newWindow;
		if (!m_window) 
		{
			FE_CORE_WARN("Camera window set to nullptr.");
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void CameraComponent::ZoomBy(float newZoom)
	{
		if (newZoom <= 0)
		{
			FE_CORE_WARN("Zoom factor must be positive. Provided: " + std::to_string(newZoom));
			return;
		}
		m_view->zoom(newZoom);
	}
}