#include "pch.h"
#include "MoveComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteColliderComponent.h"

namespace BaneAndBastion
{
    //MoveComponent
    //--------------------------------------------------------------------------------------------------------
    MoveComponent::MoveComponent(FalkonEngine::GameObject* gameObject) : Component(gameObject) {}
    //--------------------------------------------------------------------------------------------------------
    void MoveComponent::Update(float deltaTime)
    {
        CalculateDesiredDir(deltaTime);
        applyMovement(m_desiredDir, deltaTime);
    }
    //--------------------------------------------------------------------------------------------------------
    void MoveComponent::SetSpeed(float speed)
    {
        m_maxSpeed = speed;
    }
    //--------------------------------------------------------------------------------------------------------
    void MoveComponent::applyMovement(const FalkonEngine::Vector2Df desiredDir, float dt)
    {
        if (desiredDir.GetLength() < 0.0001f && m_currentVelocity.GetLength() < 0.0001f)
        {
            return;
        }
        float lerpFactor = (desiredDir.GetLength() > 0.0001f) ? m_acceleration : m_friction;

        FalkonEngine::Vector2Df targetVelocity = desiredDir * m_maxSpeed;

        m_currentVelocity.x += (targetVelocity.x - m_currentVelocity.x) * lerpFactor * dt;
        m_currentVelocity.y += (targetVelocity.y - m_currentVelocity.y) * lerpFactor * dt;

        if (m_currentVelocity.GetLength() < 0.00001f) {
            m_currentVelocity = { 0.0f, 0.0f };
        }

        FalkonEngine::Vector2Df delta = m_currentVelocity * dt;

        if (std::abs(delta.x) > m_notify_delta || std::abs(delta.y) > m_notify_delta) 
        {
            FalkonEngine::GameEvent ev;
            ev.type = FalkonEngine::GameEventType::PositionChanged; 
            ev.sender = this;
            ev.direction.x = delta.x;
            ev.direction.y = delta.y;
            ev.entityID = GetGameObject()->GetID();
            Notify(ev);
        }
    }
}