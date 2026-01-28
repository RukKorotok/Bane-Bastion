#pragma once

#include <SFML/Graphics.hpp>

namespace FalkonEngine
{
	class ColliderComponent;
	struct Collision
	{
	public:
		Collision(ColliderComponent* newFirst, ColliderComponent* newSecond, sf::FloatRect newCollisionRect) :
			m_first(newFirst), m_second(newSecond), m_collisionRect(newCollisionRect) {
		};
	private:
		ColliderComponent* m_first;
		ColliderComponent* m_second;
		sf::FloatRect m_collisionRect;
	};
}