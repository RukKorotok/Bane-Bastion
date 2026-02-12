#include "pch.h"
#include "Sprite.h"

namespace FalkonEngine
{
	//-----------------------------------------------------------------------------------------------------------
	void InitSprite(sf::Sprite& sprite, float desiredWidth, float desiredHeight, const sf::Texture& texture)
	{
		sprite.setTexture(texture);
		SetSpriteRelativeOrigin(sprite, 0.5f, 0.5f);
		SetSpriteSize(sprite, desiredWidth, desiredHeight);
		FE_APP_TRACE("Sprite initialized: Size " + std::to_string(desiredWidth) + "x" + std::to_string(desiredHeight));
	}
	//-----------------------------------------------------------------------------------------------------------
	void DrawSprite(const sf::Sprite& sprite, sf::RenderWindow& window)
	{
		window.draw(sprite);
	}
	//-----------------------------------------------------------------------------------------------------------
	sf::Vector2f GetRandomSpritePositionInRectangle(const sf::Sprite& sprite, const sf::FloatRect& rect)
	{

		sf::FloatRect bounds = sprite.getGlobalBounds();

		if (rect.width <= bounds.width * 2 || rect.height <= bounds.height * 2)
		{
			FE_CORE_WARN("GetRandomSpritePosition: Target rectangle is too small for the sprite. Returning rect center.");
			return { rect.left + rect.width / 2.f, rect.top + rect.height / 2.f };
		}

		sf::Vector2f result;
		const auto spriteWidth = sprite.getGlobalBounds().width;
		const auto spriteHeight = sprite.getGlobalBounds().height;
		result.x = rand() / (float)RAND_MAX * (rect.width - 2 * spriteWidth) + rect.left + spriteWidth;
		result.y = rand() / (float)RAND_MAX * (rect.height - 2 * spriteHeight) + rect.top + spriteHeight;
		return result;
	}
	//-----------------------------------------------------------------------------------------------------------
	void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight)
	{
		sf::FloatRect localRect = sprite.getLocalBounds();

		if (localRect.width > 0 && localRect.height > 0)
		{
			sprite.setScale(desiredWidth / localRect.width, desiredHeight / localRect.height);
		}
		else
		{
			FE_CORE_ERROR("SetSpriteSize: Sprite has no texture or empty local bounds!");
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void SetSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float originY)
	{
		sf::FloatRect spriteRect = sprite.getGlobalBounds();
		sprite.setOrigin(originX * spriteRect.width, originY * spriteRect.height);
	}
	//-----------------------------------------------------------------------------------------------------------
	void SetSpriteRandomPosition(sf::Sprite& sprite, const sf::FloatRect& rect, const std::list<sf::Sprite>& collection)
	{
		
	}
	//-----------------------------------------------------------------------------------------------------------
	bool CheckSpriteIntersection(const sf::Sprite& sprite1, const sf::Sprite& sprite2) 
	{
		sf::Vector2f pos1 = sprite1.getPosition();
		sf::Vector2f pos2 = sprite2.getPosition();

		float distanceSq = (pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y);

		float r1 = sprite1.getGlobalBounds().width / 2.0f;
		float r2 = sprite2.getGlobalBounds().width / 2.0f;
		float radiusSum = r1 + r2;

		return distanceSq < (radiusSum * radiusSum);
	}
	//-----------------------------------------------------------------------------------------------------------
	sf::Vector2f GetVectorBetweenSprites(const sf::Sprite& spriteFrom, const sf::Sprite& spriteTo) 
	{
		const auto result = spriteTo.getPosition() - spriteFrom.getPosition();
		return result;
	}
	//-----------------------------------------------------------------------------------------------------------
	float GetManhattanDistanceBetweenSprites(const sf::Sprite& spriteFrom, const sf::Sprite& spriteTo) 
	{
		const auto result = spriteTo.getPosition() - spriteFrom.getPosition();
		return std::fabs(result.x) + std::fabs(result.y);
	}
}
