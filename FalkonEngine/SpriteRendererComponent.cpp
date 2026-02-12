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
		m_scale = { 1.0f, -1.0f };

		m_transform = gameObject->GetComponent<TransformComponent>();

		FE_CORE_ASSERT(m_transform != nullptr, "SpriteRendererComponent added to GameObject '" + p_gameObject->GetName() + "' without TransformComponent!");

		FE_APP_TRACE("SpriteRendererComponent initialized for: " + p_gameObject->GetName());
	}
	//-----------------------------------------------------------------------------------------------------------
	SpriteRendererComponent::~SpriteRendererComponent()
	{
		if (m_sprite != nullptr)
		{
			delete m_sprite;
			m_sprite = nullptr;
		}
		FE_APP_TRACE("SpriteRendererComponent destroyed for: " + p_gameObject->GetName());
	}
	//-----------------------------------------------------------------------------------------------------------
	void SpriteRendererComponent::Update(float deltaTime){}
	//-----------------------------------------------------------------------------------------------------------
	void SpriteRendererComponent::Render()
	{
		if (m_sprite != nullptr && m_sprite->getTexture() != nullptr)
		{
			FE_CORE_ASSERT(m_transform != nullptr, "Transform lost in SpriteRenderer::Render!");

			m_sprite->setPosition(Convert<sf::Vector2f, Vector2Df>(m_transform->GetWorldPosition()));
			m_sprite->setRotation(m_transform->GetWorldRotation());

			Vector2Df transformScale = m_transform->GetWorldScale();

			m_sprite->setScale({ m_scale.x * transformScale.x, m_scale.y * transformScale.y });

			RenderSystem::Instance()->Render(*m_sprite);
		}
		else if (m_sprite != nullptr && m_sprite->getTexture() == nullptr)
		{
			static std::string lastWarnedObject = "";
			if (lastWarnedObject != p_gameObject->GetName())
			{
				FE_CORE_WARN("SpriteRenderer on '" + p_gameObject->GetName() + "' has no texture assigned. Skipping render.");
				lastWarnedObject = p_gameObject->GetName();
			}
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
		if (m_sprite == nullptr) 
		{
			return;
		}
		m_sprite->setTexture(newTexture);

		sf::Vector2u textureSize = newTexture.getSize();
		m_sprite->setOrigin({ 0.5f * textureSize.x, 0.5f * textureSize.y });

		FE_CORE_INFO("Texture set for '" + p_gameObject->GetName() + "'. Size: " +
			std::to_string(textureSize.x) + "x" + std::to_string(textureSize.y));
	}
	//-----------------------------------------------------------------------------------------------------------
	void SpriteRendererComponent::SetPixelSize(int newWidth, int newHeight)
	{
		if (m_sprite == nullptr || m_sprite->getTexture() == nullptr)
		{
			FE_CORE_ERROR("Cannot SetPixelSize: Sprite or Texture is null on " + p_gameObject->GetName());
			return;
		}

		auto originalSize = m_sprite->getTexture()->getSize();

		if (originalSize.x == 0 || originalSize.y == 0)
		{
			FE_CORE_ERROR("SetPixelSize failed: Texture size is zero on " + p_gameObject->GetName());
			return;
		}

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