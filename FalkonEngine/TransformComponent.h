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

		// --- (Position) ---
		/** move by offset */
		void MoveBy(const Vector2Df& offset);

		/** move by x offset and y offset */
		void MoveBy(float offsetX, float offsetY);

		/** set world position */
		void SetWorldPosition(const Vector2Df& position);
		void SetWorldPosition(float positionX, float positionY);

		/** set local position */
		void SetLocalPosition(const Vector2Df& position);
		void SetLocalPosition(float positionX, float positionY);

		// --- (Rotation) ---
		/** rotate by added angle */
		void RotateBy(float offset);

		/** set world rotation */
		void SetWorldRotation(float angle);

		/** set local rotation */
		void SetLocalRotation(float angle);

		// --- (Scale) ---
		/** scale by scaleOffset */
		void ScaleBy(const Vector2Df& scaleOffset);

		/** scale by scaleOffset x and scaleOffset y */
		void ScaleBy(float scaleX, float scaleY);

		/** set world scale */
		void SetWorldScale(const Vector2Df& scale);
		void SetWorldScale(float scaleX, float scaleY);

		/** set local scale */
		void SetLocalScale(const Vector2Df& scale);
		void SetLocalScale(float scaleX, float scaleY);

		const Vector2Df& GetWorldPosition() const;
		const Vector2Df& GetLocalPosition() const;

		const float GetWorldRotation() const;
		const float GetLocalRotation() const;

		const Vector2Df& GetWorldScale() const;
		const Vector2Df& GetLocalScale() const;

		// --- (Parent) ---
		/** set parent transform component */
		void SetParent(TransformComponent* newParent);
		TransformComponent* GetParent() const;

		const Matrix2D GetWorldTransform() const;

		// --- (Debug) ---
		// Print debug info
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