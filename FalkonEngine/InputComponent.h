#pragma once

#include "Component.h"
#include <SFML/Window.hpp> 

namespace FalkonEngine
{
	class InputComponent : public Component
	{
	public:
		InputComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		float GetHorizontalAxis() const;
		float GetVerticalAxis() const;
	private:
		float m_horizontalAxis = 0.f;
		float m_verticalAxis = 0.f;
	};
}