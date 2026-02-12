#include "pch.h"
#include "Collision.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"

namespace FalkonEngine
{
    //Collision
    //--------------------------------------------------------------------------------------------------------
    Collision::Collision(ColliderComponent* newFirst, ColliderComponent* newSecond, sf::FloatRect newCollisionRect) :
        m_first(newFirst), m_second(newSecond), m_collisionRect(newCollisionRect) 
    {
        FE_APP_TRACE("Collision object created between: " + m_first->GetGameObject()->GetName() + " and " + m_second->GetGameObject()->GetName());
    }
    //--------------------------------------------------------------------------------------------------------
    Vector2Df Collision::GetKnockbackDirection() const
    {
        auto transformA = m_first->GetGameObject()->GetComponent<TransformComponent>();
        auto transformB = m_second->GetGameObject()->GetComponent<TransformComponent>();

        if (!transformA || !transformB) 
        {
            FE_CORE_ERROR("Collision::GetKnockbackDirection: One of the GameObjects is missing a TransformComponent!");
            return { 0.0f, 0.0f };
        }

        Vector2Df posA = transformA->GetWorldPosition();
        Vector2Df posB = transformB->GetWorldPosition();

        Vector2Df direction = posA - posB;

        float lengthSq = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (lengthSq > 0.001f)
        {
            float length = std::sqrt(lengthSq);
            return { direction.x / length, direction.y / length };
        }

        return { 0.f, -1.0f };
    }
    //--------------------------------------------------------------------------------------------------------
    ColliderComponent* Collision::GetFirst() const
    {
        return m_first;
    }
    //--------------------------------------------------------------------------------------------------------
    ColliderComponent* Collision::GetSecond() const
    {
        return m_second;
    }
    //--------------------------------------------------------------------------------------------------------
    sf::FloatRect Collision::GetRect() const
    {
        return m_collisionRect;
    }
}