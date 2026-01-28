#pragma once

#include "Component.h"
#include "Vector.h"

namespace FalkonEngine
{
    class MoveComponent : public Component
    {
    public:
        MoveComponent(GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override {}

        void SetDirection(const Vector2Df& direction);
        void SetSpeed(float speed);

        void Stop() { m_currentVelocity = { 0, 0 }; m_targetDirection = { 0, 0 }; }

    private:
        Vector2Df m_targetDirection = { 0, 0 };
        Vector2Df m_currentVelocity = { 0, 0 };

        float m_maxSpeed = 300.f;
        float m_acceleration = 12.0f;
        float m_friction = 10.0f;    
        const float m_notify_delta = 0.0001f;
    };
}