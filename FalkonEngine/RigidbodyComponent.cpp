#include "pch.h"
#include "RigidbodyComponent.h"

namespace FalkonEngine
{
	RigidbodyComponent::RigidbodyComponent(GameObject* gameObject) : Component(gameObject)
	{
		m_transform = gameObject->GetComponent<TransformComponent>();
	}

	void RigidbodyComponent::Update(float deltaTime)
	{
		m_transform->MoveBy(m_linearVelocity * deltaTime);
		m_transform->RotateBy(m_angleVelocity * deltaTime);

		m_linearVelocity = (1.f - m_linearDamping * deltaTime) * m_linearVelocity;
		m_angleVelocity = (1.f - m_angleDamping * deltaTime) * m_angleVelocity;

		if (std::abs(m_linearVelocity.x) < 0.1f)
		{
			m_linearVelocity.x = 0.0f;
		}
		if (std::abs(m_linearVelocity.y) < 0.1f)
		{
			m_linearVelocity.y = 0.0f;
		}

		if (m_angleVelocity < 0.001f)
		{
			m_angleVelocity = 0.0f;
		}
	}

	void RigidbodyComponent::Render()
	{

	}

	void RigidbodyComponent::SetLinearVelocity(Vector2Df newLinearVelocity)
	{
		m_linearVelocity = newLinearVelocity;
	}
	void RigidbodyComponent::AddLinearVelocity(Vector2Df linearOffset)
	{
		m_linearVelocity = m_linearVelocity + linearOffset;
	}
	Vector2Df RigidbodyComponent::GetLinearVelocity() const
	{
		return m_linearVelocity;
	}

	void RigidbodyComponent::SetAngleVelocity(float newAngleVelocity)
	{
		m_angleVelocity = newAngleVelocity;
	}
	void RigidbodyComponent::AddAngleVelocity(float angleOffset)
	{
		m_angleVelocity += angleOffset;
	}
	float RigidbodyComponent::GetAngleVelocity() const
	{
		return m_angleVelocity;
	}

	void RigidbodyComponent::SetLinearDamping(float newLinearDamping)
	{
		m_linearDamping = newLinearDamping;
	}
	float RigidbodyComponent::GetLinearDamping() const
	{
		return m_linearDamping;
	}

	void RigidbodyComponent::SetAngleDamping(float newAngleDamping)
	{
		m_angleDamping = newAngleDamping;
	}
	float RigidbodyComponent::GetAngleDamping() const
	{
		return m_angleDamping;
	}

	void RigidbodyComponent::SetKinematic(bool newIsKinematic)
	{
		m_isKinematic = newIsKinematic;
	}
	bool RigidbodyComponent::GetKinematic() const
	{
		return m_isKinematic;
	}
	void RigidbodyComponent::ApplyImpulse(Vector2Df direction, float force)
	{
		// Ќормализуем направление на случай, если пришел "сырой" вектор
		float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (len > 0.001f) {
			direction.x /= len;
			direction.y /= len;
		}

		// ѕрикладываем силу
		AddLinearVelocity(direction * force);
	}
}