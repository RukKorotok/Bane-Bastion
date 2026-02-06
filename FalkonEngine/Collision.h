#pragma once

#include <SFML/Graphics.hpp>
#include "Vector.h"

namespace FalkonEngine
{
	class ColliderComponent;

	struct Collision
	{
	public:
		Collision(ColliderComponent* newFirst, ColliderComponent* newSecond, sf::FloatRect newCollisionRect);

		Vector2Df GetKnockbackDirection() const;

		ColliderComponent* GetFirst() const;
		ColliderComponent* GetSecond() const;
		sf::FloatRect GetRect() const;
	private:
		ColliderComponent* m_first;
		ColliderComponent* m_second;
		sf::FloatRect m_collisionRect;
	};
}