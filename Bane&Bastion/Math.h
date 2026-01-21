#pragma once

namespace sf
{
	class Sprite;
}

namespace Bane_Bastion
{

	struct Vector2D
	{
		float x = 0;
		float y = 0;
	};

	class Math
	{
	public:

		static Math& GetInstance()
		{
			static Math math;
			return math;
		}

		float GetNearest(float origin, float min, float max);
		void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight);
		void SetSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float originY);
		bool IsRectanglesCollide(Vector2D rect1Position, Vector2D rect1Size, Vector2D rect2Position, Vector2D rect2Size);
		bool IsCirclesCollide(Vector2D circle1Position, float circle1Radius, Vector2D circle2Position, float circle2Radius);
		bool IsCicleRectangleCollition(Vector2D circlePosition, float circleRadius, Vector2D rectPosition, Vector2D rectSize);
		Vector2D CalculateReboundSpeedByRectangle(Vector2D ciclePosition, float circleSize, Vector2D cicleSpeed, Vector2D rectPosition, Vector2D rectSize, float maxReboundAngle);

	private:

		Math() {}
		~Math() {}

		Math(Math const&) = delete;
		Math& operator = (Math const&) = delete;
	};
}
