#include "pch.h"
#include "MoveComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteColliderComponent.h"
#include "PlayerMoveComponent.h"

namespace BaneAndBastion
{
    //MoveComponent
    //--------------------------------------------------------------------------------------------------------
    PlayerMoveComponent::PlayerMoveComponent(FalkonEngine::GameObject* gameObject) : MoveComponent(gameObject) 
    {
        m_targetDirection = { 0.0f, 0.0f };
        m_currentVelocity = { 0.0f, 0.0f };
    }
    //--------------------------------------------------------------------------------------------------------
    void PlayerMoveComponent::Stop()
    {
        m_targetDirection = { 0.0f, 0.0f };
        m_currentVelocity = { 0.0f, 0.0f };

        FE_APP_TRACE("PlayerMoveComponent: Movement stopped.");
    }
    //--------------------------------------------------------------------------------------------------------
    void PlayerMoveComponent::SetTargetDirection(FalkonEngine::Vector2Df dir)
    {
        float lenSq = dir.x * dir.x + dir.y * dir.y;

        if (lenSq > 0.0001f)
        {
            float len = std::sqrt(lenSq);
            m_targetDirection = { dir.x / len, dir.y / len };
        }
        else
        {
            m_targetDirection = { 0.0f, 0.0f };
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void PlayerMoveComponent::CalculateDesiredDir(float dt)
    {
     m_desiredDir = m_targetDirection;
    }
    
}