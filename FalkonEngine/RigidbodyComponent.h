#pragma once

#include "TransformComponent.h"
#include "Vector.h"

namespace FalkonEngine
{
	class RigidbodyComponent : public Component
	{
	public:
		RigidbodyComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetLinearVelocity(Vector2Df newLinearVelocity);
		void AddLinearVelocity(Vector2Df linearOffset);
		Vector2Df GetLinearVelocity() const;

		void SetAngleVelocity(float newAngleVelocity);
		void AddAngleVelocity(float angleOffset);
		float GetAngleVelocity() const;

		void SetLinearDamping(float newLinearDamping);
		float GetLinearDamping() const;

		void SetAngleDamping(float newAngleDamping);
		float GetAngleDamping() const;

		void SetKinematic(bool newIsKinematic);
		bool GetKinematic() const;

		void ApplyImpulse(Vector2Df direction, float force);

	private:
		TransformComponent* m_transform;

		Vector2Df m_linearVelocity = { 0.f, 0.f };
		float m_angleVelocity = 0.f;

		float m_linearDamping = 0.f;
		float m_angleDamping = 0.f;

		bool m_isKinematic = false;
	};
}