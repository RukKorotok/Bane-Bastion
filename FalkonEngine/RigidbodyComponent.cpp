#include "pch.h"
#include "RigidbodyComponent.h"
#include <cmath>

namespace FalkonEngine
{
	//RigidbodyComponent
	//-----------------------------------------------------------------------------------------------------------
	RigidbodyComponent::RigidbodyComponent(GameObject* gameObject) : Component(gameObject)
	{
		m_transform = gameObject->GetComponent<TransformComponent>();

		FE_CORE_ASSERT(m_transform != nullptr, "RigidbodyComponent added to GameObject '" + p_gameObject->GetName() + "' without TransformComponent!");

		FE_APP_TRACE("RigidbodyComponent initialized for: " + p_gameObject->GetName());
	}
	//-----------------------------------------------------------------------------------------------------------
	void RigidbodyComponent::Update(float deltaTime)
	{
		if (deltaTime <= 0.0f) 
		{
			return;
		}

		m_transform->MoveBy(m_linearVelocity * deltaTime);
		m_transform->RotateBy(m_angleVelocity * deltaTime);

		float linearDrag = 1.f - m_linearDamping * deltaTime;
		float angleDrag = 1.f - m_angleDamping * deltaTime;

		m_linearVelocity = (linearDrag > 0.f ? linearDrag : 0.f) * m_linearVelocity;
		m_angleVelocity = (angleDrag > 0.f ? angleDrag : 0.f) * m_angleVelocity;

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
	//-----------------------------------------------------------------------------------------------------------
	void RigidbodyComponent::Render(){}
	//-----------------------------------------------------------------------------------------------------------
	void RigidbodyComponent::SetLinearVelocity(Vector2Df newLinearVelocity)
	{
		if (!std::isfinite(newLinearVelocity.x) || !std::isfinite(newLinearVelocity.y))
		{
			FE_CORE_ERROR("Attempted to set non-finite LinearVelocity on: " + p_gameObject->GetName());
			return;
		}
		m_linearVelocity = newLinearVelocity;
	}
	//-----------------------------------------------------------------------------------------------------------
	void RigidbodyComponent::AddLinearVelocity(Vector2Df linearOffset)
	{
		m_linearVelocity = m_linearVelocity + linearOffset;
	}
	//-----------------------------------------------------------------------------------------------------------
	Vector2Df RigidbodyComponent::GetLinearVelocity() const
	{
		return m_linearVelocity;
	}
	//-----------------------------------------------------------------------------------------------------------
	void RigidbodyComponent::SetAngleVelocity(float newAngleVelocity)
	{
		if (!std::isfinite(newAngleVelocity))
		{
			FE_CORE_ERROR("Attempted to set non-finite AngleVelocity on: " + p_gameObject->GetName());
			return;
		}
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
	//-----------------------------------------------------------------------------------------------------------
	void RigidbodyComponent::SetLinearDamping(float newLinearDamping)
	{
		m_linearDamping = newLinearDamping;
	}
	float RigidbodyComponent::GetLinearDamping() const
	{
		return m_linearDamping;
	}
	//-----------------------------------------------------------------------------------------------------------
	void RigidbodyComponent::SetAngleDamping(float newAngleDamping)
	{
		m_angleDamping = newAngleDamping;
	}
	float RigidbodyComponent::GetAngleDamping() const
	{
		return m_angleDamping;
	}
	//-----------------------------------------------------------------------------------------------------------
	void RigidbodyComponent::SetKinematic(bool newIsKinematic)
	{
		m_isKinematic = newIsKinematic;
	}
	//-----------------------------------------------------------------------------------------------------------
	bool RigidbodyComponent::GetKinematic() const
	{
		return m_isKinematic;
	}
	//-----------------------------------------------------------------------------------------------------------
	void RigidbodyComponent::ApplyImpulse(Vector2Df direction, float force)
	{
		float lenSq = direction.x * direction.x + direction.y * direction.y;

		if (lenSq > 0.000001f)
		{
			float len = std::sqrt(lenSq);
			Vector2Df normalizedDir = { direction.x / len, direction.y / len };
			AddLinearVelocity(normalizedDir * force);
		}
		else
		{
			FE_CORE_WARN("ApplyImpulse called with zero-length direction on: " + p_gameObject->GetName());
		}
	}
}