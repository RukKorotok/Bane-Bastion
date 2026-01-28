#include "pch.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"
#include "RenderSystem.h"

namespace FalkonEngine
{
	//SpriteRendererComponent
	//-----------------------------------------------------------------------------------------------------------
	SpriteRendererComponent::SpriteRendererComponent(GameObject* gameObject) : Component(gameObject)
	{
		m_sprite = new sf::Sprite();
		m_scale = { 1, -1 };
		m_transform = gameObject->GetComponent<TransformComponent>();
	}
	//-----------------------------------------------------------------------------------------------------------
	SpriteRendererComponent::~SpriteRendererComponent()
	{
		if (m_sprite != nullptr)
		{
			delete m_sprite;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void SpriteRendererComponent::Update(float deltaTime)
	{

	}
	//-----------------------------------------------------------------------------------------------------------
	void SpriteRendererComponent::Render()
	{
		if (m_sprite != nullptr)
		{
			m_sprite->setPosition(Convert<sf::Vector2f, Vector2Df>(m_transform->GetWorldPosition()));
			m_sprite->setRotation(m_transform->GetWorldRotation());

			auto transformScale = Convert<sf::Vector2f, Vector2Df>(m_transform->GetWorldScale());
			m_sprite->setScale({ m_scale.x * transformScale.x, m_scale.y * transformScale.y });
			RenderSystem::Instance()->Render(*m_sprite);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	const sf::Sprite* SpriteRendererComponent::GetSprite() const
	{
		return m_sprite;
	}
	//-----------------------------------------------------------------------------------------------------------
	void SpriteRendererComponent::SetTexture(const sf::Texture& newTexture)
	{
		m_sprite->setTexture(newTexture);
		auto textureSize = m_sprite->getTexture()->getSize();
		m_sprite->setOrigin({ 0.5f * textureSize.x, 0.5f * textureSize.y });
	}
	//-----------------------------------------------------------------------------------------------------------
	void SpriteRendererComponent::SetPixelSize(int newWidth, int newHeight)
	{
		auto originalSize = m_sprite->getTexture()->getSize();
		m_scale = { (float)newWidth / (float)originalSize.x, -(float)newHeight / (float)originalSize.y };
	}
	//-----------------------------------------------------------------------------------------------------------
	void SpriteRendererComponent::FlipX(bool flip)
	{
		if (flip != m_isFlipX)
		{
			m_scale = { -m_scale.x, m_scale.y };

			m_isFlipX = flip;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void SpriteRendererComponent::FlipY(bool flip)
	{
		if (flip != m_isFlipY)
		{
			m_scale = { m_scale.x, -m_scale.y };

			m_isFlipY = flip;
		}
	}
}