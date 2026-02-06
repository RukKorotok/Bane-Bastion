#include "pch.h"
#include "TransformComponent.h"
#include <cmath>
#include <iostream>

namespace FalkonEngine
{
	//TransformComponent
	//-----------------------------------------------------------------------------------------------------------
	TransformComponent::TransformComponent(GameObject* gameObject) : Component(gameObject) {}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::Update(float deltaTime)
	{

	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::Render() {}
	// --- (Position) ---
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::MoveBy(const Vector2Df& offset)
	{
		MoveBy(offset.x, offset.y);
		m_isUpdated = false;
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::MoveBy(float offsetX, float offsetY)
	{
		m_localPosition.x += offsetX;
		m_localPosition.y += offsetY;
		m_isUpdated = false;
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::SetWorldPosition(const Vector2Df& position)
	{
		SetWorldPosition(position.x, position.y);
		m_isUpdated = false;
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::SetWorldPosition(float positionX, float positionY)
	{
		Vector2Df newPosition = { positionX, positionY };
		if (m_parent == nullptr)
		{
			m_localPosition = newPosition;
		}
		else
		{
			Matrix2D newWorldTransform = createTransform(newPosition, GetWorldRotation(), GetWorldScale());
			m_localTransform = m_parent->GetWorldTransform().GetInversed() * newWorldTransform;

			setLocalInfoFrom(m_localTransform);

			m_isUpdated = false;
			updateLocalTransform();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::SetLocalPosition(const Vector2Df& position)
	{
		SetLocalPosition(position.x, position.y);
		m_isUpdated = false;
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::SetLocalPosition(float positionX, float positionY)
	{
		m_localPosition.x = positionX;
		m_localPosition.y = positionY;
		m_isUpdated = false;
	}
	// --- (Rotation) ---
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::RotateBy(float offset)
	{
		m_localRotation = m_localRotation + offset;
		while (m_localRotation >= 360)
		{
			m_localRotation -= 360;
		}
		while (m_localRotation < 0)
		{
			m_localRotation += 360;
		}
		m_isUpdated = false;
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::SetWorldRotation(float angle)
	{
		if (m_parent == nullptr)
		{
			m_localRotation = angle;
		}
		else
		{
			Matrix2D newWorldTransform = createTransform(GetWorldPosition(), angle, GetWorldScale());
			m_localTransform = m_parent->GetWorldTransform().GetInversed() * newWorldTransform;

			setLocalInfoFrom(m_localTransform);

			m_isUpdated = false;
			updateLocalTransform();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::SetLocalRotation(float angle)
	{
		m_localRotation = angle;
		while (m_localRotation >= 360)
		{
			m_localRotation -= 360;
		}
		while (m_localRotation < 0)
		{
			m_localRotation += 360;
		}
		m_isUpdated = false;
	}
	// --- (Scale) ---
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::ScaleBy(const Vector2Df& scale)
	{
		ScaleBy(scale.x, scale.y);
		m_isUpdated = false;
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::ScaleBy(float scaleX, float scaleY)
	{
		m_localScale.x *= scaleX;
		m_localScale.y *= scaleY;
		m_isUpdated = false;
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::SetWorldScale(const Vector2Df& scale)
	{
		SetWorldScale(scale.x, scale.y);
		m_isUpdated = false;
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::SetWorldScale(float scaleX, float scaleY)
	{
		Vector2Df newScale = { scaleX, scaleY };
		if (m_parent == nullptr)
		{
			m_localScale = newScale;
		}
		else
		{
			Matrix2D newWorldTransform = createTransform(GetWorldPosition(), GetWorldRotation(), newScale);
			m_localTransform = m_parent->GetWorldTransform().GetInversed() * newWorldTransform;

			setLocalInfoFrom(m_localTransform);

			m_isUpdated = false;
			updateLocalTransform();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::SetLocalScale(const Vector2Df& scale)
	{
		SetLocalScale(scale.x, scale.y);
		m_isUpdated = false;
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::SetLocalScale(float scaleX, float scaleY)
	{
		m_localScale.x = scaleX;
		m_localScale.y = scaleY;
		m_isUpdated = false;
	}
	//-----------------------------------------------------------------------------------------------------------
	const Vector2Df& TransformComponent::GetWorldPosition() const
	{
		updateLocalTransform();
		if (m_parent == nullptr)
		{
			return GetLocalPosition();
		}

		setWorldInfoFrom(m_parent->GetWorldTransform() * m_localTransform);
		return m_position;
	}
	//-----------------------------------------------------------------------------------------------------------
	const Vector2Df& TransformComponent::GetLocalPosition() const
	{
		updateLocalTransform();
		return m_localPosition;
	}
	//-----------------------------------------------------------------------------------------------------------
	const float TransformComponent::GetWorldRotation() const
	{
		updateLocalTransform();
		if (m_parent == nullptr)
		{
			return GetLocalRotation();
		}

		setWorldInfoFrom(m_parent->GetWorldTransform() * m_localTransform);
		return m_rotation;
	}
	//-----------------------------------------------------------------------------------------------------------
	const float TransformComponent::GetLocalRotation() const
	{
		updateLocalTransform();
		return m_localRotation;
	}
	//-----------------------------------------------------------------------------------------------------------
	const Vector2Df& TransformComponent::GetWorldScale() const
	{
		updateLocalTransform();
		if (m_parent == nullptr)
		{
			return GetLocalScale();
		}

		setWorldInfoFrom(m_parent->GetWorldTransform() * m_localTransform);
		return m_scale;
	}
	//-----------------------------------------------------------------------------------------------------------
	const Vector2Df& TransformComponent::GetLocalScale() const
	{
		updateLocalTransform();
		return m_localScale;
	}
	// --- (Parent) ---
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::SetParent(TransformComponent* newParent)
	{
		if (m_parent == nullptr && newParent != nullptr)
		{
			newParent->p_gameObject->AddChild(p_gameObject);
			m_localTransform = newParent->GetWorldTransform().GetInversed() * m_localTransform;
		}
		else if (m_parent != nullptr && newParent == nullptr)
		{
			m_parent->p_gameObject->RemoveChild(p_gameObject);
			m_localTransform = m_parent->GetWorldTransform() * m_localTransform;
		}
		else if (m_parent != nullptr && newParent != nullptr)
		{
			m_parent->p_gameObject->RemoveChild(p_gameObject);
			newParent->p_gameObject->AddChild(p_gameObject);
			m_localTransform = newParent->GetWorldTransform().GetInversed() * (m_parent->GetWorldTransform() * m_localTransform);
		}

		setLocalInfoFrom(m_localTransform);

		m_parent = newParent;
		m_isUpdated = false;
	}
	//-----------------------------------------------------------------------------------------------------------
	TransformComponent* TransformComponent::GetParent() const
	{
		return m_parent;
	}
	//-----------------------------------------------------------------------------------------------------------
	const Matrix2D TransformComponent::GetWorldTransform() const
	{
		updateLocalTransform();

		if (m_parent == nullptr)
		{
			return m_localTransform;
		}

		return m_parent->GetWorldTransform() * m_localTransform;
	}
	// --- (Debug) ---
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::Print() const
	{
		std::cout << "Transform name : " << p_gameObject->GetName() << std::endl;

		std::cout << "Position" << std::endl;
		std::cout << "  World: (" << GetWorldPosition().x << ", " << GetWorldPosition().y << ")" << std::endl;
		std::cout << "  Local: (" << GetLocalPosition().x << ", " << GetLocalPosition().y << ")" << std::endl;

		std::cout << "Rotation" << std::endl;
		std::cout << "  World: " << GetWorldRotation() << std::endl;
		std::cout << "  Local: " << GetLocalRotation() << std::endl;

		std::cout << "Scale" << std::endl;
		std::cout << "  World: (" << GetWorldScale().x << ", " << GetWorldScale().y << ")" << std::endl;
		std::cout << "  Local: (" << GetLocalScale().x << ", " << GetLocalScale().y << ")" << std::endl;
	}
	//private
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::setWorldInfoFrom(const Matrix2D& transform) const
	{
		m_position.x = transform.GetMatrix()[0][2];
		m_position.y = transform.GetMatrix()[1][2];

		m_scale.x = std::sqrt(transform.GetMatrix()[0][0] * transform.GetMatrix()[0][0] + transform.GetMatrix()[1][0] * transform.GetMatrix()[1][0]);
		m_scale.y = std::sqrt(transform.GetMatrix()[0][1] * transform.GetMatrix()[0][1] + transform.GetMatrix()[1][1] * transform.GetMatrix()[1][1]);

		m_rotation = static_cast<float>(std::atan2(transform.GetMatrix()[0][1], transform.GetMatrix()[0][0]) * 180 / 3.14159265);
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::setLocalInfoFrom(const Matrix2D& transform) const
	{
		m_localPosition.x = transform.GetMatrix()[0][2];
		m_localPosition.y = transform.GetMatrix()[1][2];
		
		m_localScale.x = std::sqrt(transform.GetMatrix()[0][0] * transform.GetMatrix()[0][0] + transform.GetMatrix()[1][0] * transform.GetMatrix()[1][0]);
		m_localScale.y = std::sqrt(transform.GetMatrix()[0][1] * transform.GetMatrix()[0][1] + transform.GetMatrix()[1][1] * transform.GetMatrix()[1][1]);
		
		m_localRotation = static_cast<float>(std::atan2(transform.GetMatrix()[0][1], transform.GetMatrix()[0][0]) * 180 / 3.14159265);
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::updateLocalTransform() const
	{
		if (!m_isUpdated)
		{
			m_localTransform = createTransform(m_localPosition, m_localRotation, m_localScale);
			m_isUpdated = true;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void TransformComponent::updateLocalTransform(const Vector2Df& position, float rotation, const Vector2Df& scale) const
	{
		if (!m_isUpdated)
		{
			m_localTransform = createTransform(position, rotation, scale);
			m_isUpdated = true;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	Matrix2D TransformComponent::createTransform(const Vector2Df& position, float rotation, const Vector2Df& scale) const
	{
		return Matrix2D(position, rotation, scale);
	}
}