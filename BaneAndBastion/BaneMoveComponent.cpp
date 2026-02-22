#include "pch.h"

#include "BaneMoveComponent.h"
#include "GameObject.h"

namespace BaneAndBastion
{
    BaneMoveComponent::BaneMoveComponent(FalkonEngine::GameObject* gameObject)
        : MoveComponent(gameObject), m_isActive(false)
    {
        // Initialize movement vectors and physical constants
        m_currentVelocity = { 0.0f, 0.0f };
        m_desiredDir = { 0.0f, 0.0f };
        m_acceleration = 30.0f;
        m_friction = 50.0f;
    }

    void BaneMoveComponent::LaunchToTarget(const FalkonEngine::Vector2Df& startPos,
        const FalkonEngine::Vector2Df& targetPos)
    {
        // Set the initial and target positions and activate the movement logic
        m_startPose = startPos;
        m_calculatedTarget = targetPos;
        m_isActive = true;
    }

    void BaneMoveComponent::Stop()
    {
        // Immediately halt all movement and disable the active state
        m_isActive = false;
        m_currentVelocity = { 0.0f, 0.0f };
        m_desiredDir = { 0.0f, 0.0f };
    }

    void BaneMoveComponent::CalculateDesiredDir(float dt)
    {
        // If the component is inactive, only handle friction and position synchronization
        if (!m_isActive)
        {
            ApplyFriction(dt);
            // Synchronize virtual position even when stationary to account for residual velocity
            m_startPose.x += m_currentVelocity.x * dt;
            m_startPose.y += m_currentVelocity.y * dt;
            m_desiredDir = { 0.0f, 0.0f };
            return;
        }

        // Calculate the vector pointing towards the destination
        FalkonEngine::Vector2Df toStep = m_calculatedTarget - m_startPose;
        float distSq = (toStep.x * toStep.x) + (toStep.y * toStep.y);

        // Check if the entity is outside the arrival threshold radius
        if (distSq > m_arrivalThresholdSq)
        {
            // Calculate normalized direction towards the target
            float len = std::sqrt(distSq);
            m_desiredDir = { toStep.x / len, toStep.y / len };

            // Apply acceleration to current velocity based on desired direction
            m_currentVelocity.x += m_desiredDir.x * m_acceleration * dt;
            m_currentVelocity.y += m_desiredDir.y * m_acceleration * dt;

            // Speed clamping logic to maintain maximum movement velocity
            float maxSpeed = 400.0f;
            float currentSpeed = std::sqrt(m_currentVelocity.x * m_currentVelocity.x + m_currentVelocity.y * m_currentVelocity.y);
            if (currentSpeed > maxSpeed)
            {
                m_currentVelocity.x = (m_currentVelocity.x / currentSpeed) * maxSpeed;
                m_currentVelocity.y = (m_currentVelocity.y / currentSpeed) * maxSpeed;
            }
        }
        else
        {
            // Target reached: deactivate movement and broadcast completion event
            m_isActive = false;
            m_desiredDir = { 0.0f, 0.0f };

            FalkonEngine::GameEvent ev;
            ev.type = FalkonEngine::GameEventType::MovementFinished;
            ev.entityID = GetGameObject()->GetID();
            ev.sender = this;
            Notify(ev);
        }

        // Apply friction to the velocity vector
        ApplyFriction(dt);

        // Update the virtual tracking position based on the final calculated velocity
        m_startPose.x += m_currentVelocity.x * dt;
        m_startPose.y += m_currentVelocity.y * dt;
    }

    void BaneMoveComponent::ApplyFriction(float dt) {
        // Calculate current scalar speed
        float speed = std::sqrt(m_currentVelocity.x * m_currentVelocity.x + m_currentVelocity.y * m_currentVelocity.y);

        if (speed > 0) {
            // Apply linear drop to speed over time
            float drop = m_friction * dt;
            float newSpeed = speed - drop;
            if (newSpeed < 0) newSpeed = 0;

            // Scale the velocity vector by the reduction factor
            float factor = newSpeed / speed;
            m_currentVelocity.x *= factor;
            m_currentVelocity.y *= factor;
        }
    }
}