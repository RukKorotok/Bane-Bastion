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
    PlayerMoveComponent::PlayerMoveComponent(FalkonEngine::GameObject* gameObject) : MoveComponent(gameObject) {}
    //--------------------------------------------------------------------------------------------------------
    void PlayerMoveComponent::Stop()
    {
        m_targetDirection = { 0.0f, 0.0f };
        m_currentVelocity = { 0.0f, 0.0f };
    }
    //--------------------------------------------------------------------------------------------------------
    void PlayerMoveComponent::SetTargetDirection(FalkonEngine::Vector2Df dir)
    {
        float lenSq = (dir.GetLength());

        if (lenSq > 1.0f) 
        {
            dir.Normalize();
        }

        m_targetDirection = dir;
    }
    //--------------------------------------------------------------------------------------------------------
    void PlayerMoveComponent::CalculateDesiredDir(float dt)
    {
     m_desiredDir = m_targetDirection;
    }
    
}