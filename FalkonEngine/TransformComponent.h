#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "GameObject.h"
#include "Vector.h"
#include "Matrix2D.h"

namespace FalkonEngine
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void MoveBy(const Vector2Df& offset);
		void MoveBy(float offsetX, float offsetY);
		void SetWorldPosition(const Vector2Df& position);
		void SetWorldPosition(float positionX, float positionY);
		void SetLocalPosition(const Vector2Df& position);
		void SetLocalPosition(float positionX, float positionY);

		void RotateBy(float offset);
		void SetWorldRotation(float angle);
		void SetLocalRotation(float angle);

		void ScaleBy(const Vector2Df& scaleOffset);
		void ScaleBy(float scaleX, float scaleY);
		void SetWorldScale(const Vector2Df& scale);
		void SetWorldScale(float scaleX, float scaleY);
		void SetLocalScale(const Vector2Df& scale);
		void SetLocalScale(float scaleX, float scaleY);

		const Vector2Df& GetWorldPosition() const;
		const Vector2Df& GetLocalPosition() const;

		const float GetWorldRotation() const;
		const float GetLocalRotation() const;

		const Vector2Df& GetWorldScale() const;
		const Vector2Df& GetLocalScale() const;

		void SetParent(TransformComponent* newParent);
		TransformComponent* GetParent() const;

		const Matrix2D GetWorldTransform() const;
		void Print() const;

	private:
		TransformComponent* m_parent = nullptr;

		mutable Matrix2D m_localTransform;
		mutable bool m_isUpdated = false;

		mutable Vector2Df m_localPosition = { 0, 0 };
		mutable float m_localRotation = 0.f;
		mutable Vector2Df m_localScale = { 1, 1 };

		mutable Vector2Df m_position = { 0, 0 };
		mutable float m_rotation = 0.f;
		mutable Vector2Df m_scale = { 1, 1 };

		void setWorldInfoFrom(const Matrix2D& transform) const;
		void setLocalInfoFrom(const Matrix2D& transform) const;
		void updateLocalTransform() const;
		void updateLocalTransform(const Vector2Df& position, float rotation, const Vector2Df& scale) const;
		Matrix2D createTransform(const Vector2Df& position, float rotation, const Vector2Df& scale) const;
	};
}