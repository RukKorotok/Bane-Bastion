#pragma once

#include "Component.h"
#include "Vector.h"
#include <SFML/Window.hpp>

namespace FalkonEngine
{
	class InputComponent : public Component
	{
	public:
		InputComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		//** Get horizontal axis value */
		float GetHorizontalAxis() const;

		//** Get vertical axis value */
		float GetVerticalAxis() const;

	private:
		float m_horizontalAxis = 0.f;
		float m_verticalAxis = 0.f;
		Vector2Df m_lastMousePos;

	};
}