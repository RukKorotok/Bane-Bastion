#include "pch.h"
#include "MoveComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"

namespace FalkonEngine
{
    //MoveComponent
    //--------------------------------------------------------------------------------------------------------
    MoveComponent::MoveComponent(GameObject* gameObject) : Component(gameObject) {}
    //--------------------------------------------------------------------------------------------------------
    void MoveComponent::SetDirection(const Vector2Df& direction)
    {
        m_targetDirection = GetNormalized(direction);
    }
    //--------------------------------------------------------------------------------------------------------
    void MoveComponent::SetSpeed(float speed)
    {
        m_maxSpeed = speed;
    }
    //--------------------------------------------------------------------------------------------------------
    void MoveComponent::Update(float deltaTime)
    {
        auto transform = GetGameObject()->GetComponent<TransformComponent>();
        float lerpFactor;

        if (!transform)
        {
            return;
        }

        Vector2Df targetVelocity = m_targetDirection * m_maxSpeed;

        if (m_targetDirection.x != 0 || m_targetDirection.y != 0) {
            lerpFactor = m_acceleration;
        }
        else
        {
            lerpFactor = m_friction;
        }

        m_currentVelocity.x += (targetVelocity.x - m_currentVelocity.x) * lerpFactor * deltaTime;
        m_currentVelocity.y += (targetVelocity.y - m_currentVelocity.y) * lerpFactor * deltaTime;

        float dx = m_currentVelocity.x * deltaTime;
        float dy = m_currentVelocity.y * deltaTime;

        if (std::abs(dx) > m_notify_delta || std::abs(dy) > m_notify_delta) {

            FalkonEngine::GameEvent ev;
            ev.type = FalkonEngine::GameEventType::PositionChanged;
            ev.sender = this;
            ev.direction = { dx, dy };

            Notify(ev);
        }
    }
}