#include <cstdlib>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Game.h"

namespace Bane_Bastion
{
	//------------------------------------------------------------------------------------------------------------
	float Math::GetNearest(float origin, float min, float max)
	{
		if (origin < min)
		{
			return min;
		}
		else if (origin > max)
		{
			return max;
		}
		else
		{
			return origin;
		}
	}
	//------------------------------------------------------------------------------------------------------------
	void Math::SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight)
	{
		sf::FloatRect spriteRect = sprite.getLocalBounds();
		sf::Vector2f scale = {desiredWidth / spriteRect.width, desiredHeight / spriteRect.height};
		sprite.setScale(scale);
	}
	//------------------------------------------------------------------------------------------------------------
	void Math::SetSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float originY)
	{
		sf::FloatRect spriteRect = sprite.getLocalBounds();
		sprite.setOrigin(originX * spriteRect.width, originY * spriteRect.height);
	}
	//------------------------------------------------------------------------------------------------------------
	bool Math::IsRectanglesCollide(Vector2D rect1Position, Vector2D rect1Size, Vector2D rect2Position, Vector2D rect2Size)
	{
		float dx = (float)fabs(rect1Position.x - rect2Position.x);
		float dy = (float)fabs(rect1Position.y - rect2Position.y);

		return (dx <= (rect1Size.x + rect2Size.x) / 2.f &&
			dy <= (rect1Size.y + rect2Size.y) / 2.f);
	}
	//------------------------------------------------------------------------------------------------------------
	bool Math::IsCirclesCollide(Vector2D circle1Position, float circle1Radius, Vector2D circle2Position, float circle2Radius)
	{
		float squareDistance = (circle1Position.x - circle2Position.x) *
			(circle1Position.x - circle2Position.x) +
			(circle1Position.y - circle2Position.y) * (circle1Position.y - circle2Position.y);
		float squareRadiusSum = (circle1Radius + circle2Radius) * (circle1Radius + circle2Radius);

		return squareDistance <= squareRadiusSum;
	}
	//------------------------------------------------------------------------------------------------------------
	bool Math::IsCicleRectangleCollition(Vector2D circlePosition, float circleRadius, Vector2D rectPosition, Vector2D rectSize)
	{
		float closestPointX = GetNearest(circlePosition.x, rectPosition.x - rectSize.x * 0.5f, rectPosition.x + rectSize.x * 0.5f);
		float closestPointY = GetNearest(circlePosition.y, rectPosition.y - rectSize.y * 0.5f, rectPosition.y + rectSize.y * 0.5f);
		float distanceX = circlePosition.x - closestPointX;
		float distanceY = circlePosition.y - closestPointY;
		float distanceSquered = (distanceX * distanceX) + (distanceY * distanceY);

		return distanceSquered <= (circleRadius * circleRadius);
	}
	//------------------------------------------------------------------------------------------------------------
	Vector2D Math::CalculateReboundSpeedByRectangle(Vector2D ciclePosition, float circleSize, Vector2D cicleSpeed, Vector2D rectPosition, Vector2D rectSize, float maxReboundAngle)
	{
		const float normal_angle = 90.0f;
		float normalizeXPosition = 0.0f;
		float reboundAngle = 0.0f;
		float speedMagnitude = 0.0f;
		Vector2D newDirection;

		Vector2D delta = {
			ciclePosition.x - rectPosition.x,
			ciclePosition.y - rectPosition.y
		};
		Vector2D halfRectSize = { rectSize.x / 2.0f, rectSize.y / 2.0f };

		float overlapX = halfRectSize.x + circleSize - std::abs(delta.x);
		float overlapY = halfRectSize.y + circleSize - std::abs(delta.y);

		if (overlapX <= 0 || overlapY <= 0) 
		{
			return cicleSpeed; 
		}

		if (overlapX < overlapY) 
		{
			if (delta.x < 0.0f)
			{
				return Vector2D({ abs(cicleSpeed.x) * (-1.0f), cicleSpeed.y });
			}
			else
			{
				return Vector2D({ abs(cicleSpeed.x), cicleSpeed.y });
			}
		}
		else {
			
			normalizeXPosition = delta.x / halfRectSize.x * 0.5f;
			reboundAngle = normalizeXPosition * maxReboundAngle * 3.14159f / 180.0f;

			if (cicleSpeed.y > 0.0f) 
			{
				newDirection = Vector2D({ sin(reboundAngle), -cos(reboundAngle) });
			}
			else {
				newDirection = Vector2D({ sin(reboundAngle), cos(reboundAngle) });
			}

			speedMagnitude = std::sqrt(cicleSpeed.x * cicleSpeed.x + cicleSpeed.y * cicleSpeed.y);

			return Vector2D({ newDirection.x * speedMagnitude, newDirection.y * speedMagnitude });
		}
	}
};

