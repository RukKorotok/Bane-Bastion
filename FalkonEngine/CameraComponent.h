#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "TransformComponent.h"

namespace FalkonEngine
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(GameObject* gameObject);
		~CameraComponent();

		void Update(float deltaTime) override;
		void Render() override;

		// --- (Rendering) ---
		/** Set window */
		void SetWindow(sf::RenderWindow* newWindow);

		/** Set camera zoom */
		void ZoomBy(float newZoom);

		/** Set resolution*/
		void SetBaseResolution(int width, int height);

	private:
		TransformComponent* m_transform;
		sf::RenderWindow* m_window;
		sf::View* m_view;
	};
}