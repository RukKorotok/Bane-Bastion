#pragma once

#include "ColliderComponent.h"
#include "SpriteRendererComponent.h"
#include "RenderSystem.h"
#include <SFML/Graphics/Sprite.hpp>

namespace FalkonEngine
{
	class SpriteColliderComponent : public ColliderComponent
	{
	public:
		SpriteColliderComponent(GameObject* gameObject);
		~SpriteColliderComponent();
		const sf::FloatRect& GetBounds() const;

		/** * @brief Get objectSize by grid size.
		 * @param ppu Pixels Per Unit.
		 */
		Vector2Df GetSizeInUnits(float ppu) const;

		void Update(float deltaTime) override;
		void Render() override;
	private:
		const sf::Sprite* m_sprite;
	};
}
