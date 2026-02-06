#include "AIMoveComponent.h"

namespace BaneAndBastion
{
	//AIMoveComponent
	//-----------------------------------------------------------------------------------------------------------
	AIMoveComponent::AIMoveComponent(FalkonEngine::GameObject* gameObject) : MoveComponent(gameObject) {}
    //-----------------------------------------------------------------------------------------------------------
    void AIMoveComponent::Stop()
    {
        m_currentVelocity = { 0.0f, 0.0f };
    }
    //-----------------------------------------------------------------------------------------------------------
    void AIMoveComponent::MoveTowards(FalkonEngine::Vector2Df start, FalkonEngine::Vector2Df target) 
    {
        m_startPos = start;
        m_targetPos = target;
        m_active = true;
    }
    //-----------------------------------------------------------------------------------------------------------
    void AIMoveComponent::CalculateDesiredDir(float dt)
    {
        if (!m_active) 
        {
            m_desiredDir = { 0, 0 };
            return;
        }

        FalkonEngine::Vector2Df toTarget = { m_targetPos.x - m_startPos.x, m_targetPos.y - m_startPos.y };
        float distSq = (toTarget.x * toTarget.x) + (toTarget.y * toTarget.y);

        if (distSq > 4.0f) 
        {
            toTarget.Normalize();
            m_desiredDir = toTarget;

            m_startPos.x += m_currentVelocity.x * dt;
            m_startPos.y += m_currentVelocity.y * dt;
        }
        else 
        {
            m_desiredDir = { 0, 0 };
            m_active = false;

            FalkonEngine::GameEvent ev;
            ev.type = FalkonEngine::GameEventType::MovementFinished;
            ev.sender = this;
            this->Notify(ev);
        }
    }
}
