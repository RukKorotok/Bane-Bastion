#include "pch.h"
#include "SpriteColliderComponent.h"

namespace FalkonEngine
{
	//SpriteColliderComponent
	//-----------------------------------------------------------------------------------------------------------
	SpriteColliderComponent::SpriteColliderComponent(GameObject* gameObject) : ColliderComponent(gameObject)
	{
		auto spriteRenderer = gameObject->GetComponent<SpriteRendererComponent>();

		if (spriteRenderer == nullptr)
		{
			FE_CORE_ERROR("SpriteColliderComponent: SpriteRendererComponent required but not found on " + p_gameObject->GetName());
			FE_CORE_ASSERT(false, "Cannot add SpriteCollider without SpriteRenderer!");
			return;
		}

		m_sprite = spriteRenderer->GetSprite();

		FE_CORE_ASSERT(m_sprite != nullptr, "SpriteRenderer on " + p_gameObject->GetName() + " has no SFML Sprite!");

		PhysicsSystem::Instance()->Subscribe(this);

		FE_APP_TRACE("SpriteColliderComponent initialized for: " + p_gameObject->GetName());
	}
	//-----------------------------------------------------------------------------------------------------------
	SpriteColliderComponent::~SpriteColliderComponent()
	{
		PhysicsSystem::Instance()->Unsubscribe(this);
		FE_APP_TRACE("SpriteColliderComponent destroyed for: " + p_gameObject->GetName());
	}
	//-----------------------------------------------------------------------------------------------------------
	const sf::FloatRect& SpriteColliderComponent::GetBounds() const
	{
		return m_bounds;
	}
	//-----------------------------------------------------------------------------------------------------------
	Vector2Df SpriteColliderComponent::GetSizeInUnits(float ppu) const
	{
		if (ppu <= 0.0f)
		{
			FE_CORE_WARN("GetSizeInUnits: ppu must be greater than zero. Setting to 1.0f");
			ppu = 1.0f;
		}
		return { m_bounds.width / ppu, m_bounds.height / ppu };
	}
	//-----------------------------------------------------------------------------------------------------------
	void SpriteColliderComponent::Update(float deltaTime)
	{
		if (m_sprite)
		{
			m_bounds = m_sprite->getGlobalBounds();
		}
		else
		{
			FE_CORE_ERROR("SpriteColliderComponent: lost reference to Sprite in Update! GameObject: " + p_gameObject->GetName());
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void SpriteColliderComponent::Render()
	{
		sf::RectangleShape rectangle(sf::Vector2f(m_bounds.width, m_bounds.height));
		rectangle.setPosition(m_bounds.left, m_bounds.top);
		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color::Red);
		rectangle.setOutlineThickness(1.0f);

		//Debug vis
		//RenderSystem::Instance()->Render(rectangle);
	}
}