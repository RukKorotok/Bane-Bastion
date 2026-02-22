#include "pch.h"
#include "AIMoveComponent.h"
#include <string>

namespace BaneAndBastion
{
    // AIMoveComponent
    // -----------------------------------------------------------------------------------------------------------
    AIMoveComponent::AIMoveComponent(FalkonEngine::GameObject* gameObject) : MoveComponent(gameObject) {}

    // -----------------------------------------------------------------------------------------------------------
    void AIMoveComponent::Stop()
    {
        // Reset current velocity vector for an immediate halt
        m_currentVelocity = { 0.0f, 0.0f };
    }

    // -----------------------------------------------------------------------------------------------------------
    void AIMoveComponent::MoveTowards(FalkonEngine::Vector2Df start, FalkonEngine::Vector2Df target)
    {
        // Set start and end points for the current movement segment
        m_startPos = start;
        m_targetPos = target;
        // Enable direction calculation logic in CalculateDesiredDir
        m_active = true;
    }

    // -----------------------------------------------------------------------------------------------------------
    void AIMoveComponent::CalculateDesiredDir(float dt)
    {
        // If m_active flag is not set, no movement is required
        if (!m_active)
        {
            m_desiredDir = { 0, 0 };
            return;
        }

        // Calculate the direction vector towards the target
        FalkonEngine::Vector2Df toTarget = { m_targetPos.x - m_startPos.x, m_targetPos.y - m_startPos.y };

        // Use squared distance (x^2 + y^2) for optimization to avoid expensive sqrt() calls
        float distSq = (toTarget.x * toTarget.x) + (toTarget.y * toTarget.y);

        // Threshold check: if distance to target is greater than 2 units (sqrt(4))
        if (distSq > 4.0f)
        {
            toTarget.Normalize();
            // Set normalized vector for the base MoveComponent to process
            m_desiredDir = toTarget;

            // Integrate current velocity into m_startPos for a smooth reference point update
            m_startPos.x += m_currentVelocity.x * dt;
            m_startPos.y += m_currentVelocity.y * dt;
        }
        else
        {
            // Target reached: reset direction and deactivate seeking mode
            m_desiredDir = { 0, 0 };
            m_active = false;

            // Initialize and dispatch event for movement completion
            FalkonEngine::GameEvent ev;
            ev.type = FalkonEngine::GameEventType::MovementFinished;
            ev.sender = this;
            this->Notify(ev);

            FE_APP_TRACE("AIMoveComponent: Target reached by entity " + std::to_string(ev.entityID));
        }
    }
}