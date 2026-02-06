#pragma once

#include "Component.h"
#include "Vector.h"

namespace BaneAndBastion
{
    class MoveComponent : public FalkonEngine::Component
    {
    public:
        MoveComponent(FalkonEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override {}

        void SetSpeed(float speed);

        virtual void Stop() {}

    protected:
        void applyMovement(const FalkonEngine::Vector2Df desiredDir, float dt);

        virtual void CalculateDesiredDir(float dt) = 0;

        FalkonEngine::Vector2Df m_desiredDir{ 0, 0 };
        FalkonEngine::Vector2Df m_currentVelocity{ 0, 0 };
        float m_maxSpeed = 200.0f;
        float m_acceleration = 5.0f;
        float m_friction = 8.0f;
        float m_notify_delta = 0.001f;
    };
}