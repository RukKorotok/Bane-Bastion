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
			std::cout << "SpriteRenderer required to SpriteCollider." << std::endl;
			gameObject->RemoveComponent(this);
			return;
		}

		m_sprite = gameObject->GetComponent<SpriteRendererComponent>()->GetSprite();
		PhysicsSystem::Instance()->Subscribe(this);
	}
	//-----------------------------------------------------------------------------------------------------------
	SpriteColliderComponent::~SpriteColliderComponent()
	{
		if (&m_bounds != nullptr)
		{
			std::destroy_at(&m_bounds);
		}
		PhysicsSystem::Instance()->Unsubscribe(this);
	}
	//-----------------------------------------------------------------------------------------------------------
	const sf::FloatRect& SpriteColliderComponent::GetBounds() const
	{
		return m_bounds;
	}
	//-----------------------------------------------------------------------------------------------------------
	Vector2Df SpriteColliderComponent::GetSizeInUnits(float ppu) const
	{
		return { m_bounds.width / ppu, m_bounds.height / ppu };
	}
	//-----------------------------------------------------------------------------------------------------------
	void SpriteColliderComponent::Update(float deltaTime)
	{
		m_bounds = m_sprite->getGlobalBounds();
	}
	//-----------------------------------------------------------------------------------------------------------
	void SpriteColliderComponent::Render()
	{
		sf::RectangleShape rectangle(sf::Vector2f(m_bounds.width, m_bounds.height));
		rectangle.setPosition(m_bounds.left, m_bounds.top);
		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color::Red);
		rectangle.setOutlineThickness(4);
		//Debugvis
		//RenderSystem::Instance()->Render(rectangle);
	}
}