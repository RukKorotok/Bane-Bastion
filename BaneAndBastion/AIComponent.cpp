#include "pch.h"

#include "AIComponent.h"
#include "GameObject.h"

namespace BaneAndBastion 
{
    //AIComponent
    //--------------------------------------------------------------------------------------------------------
    AIComponent::AIComponent(FalkonEngine::GameObject* gameObject) : Component(gameObject) {}
    //--------------------------------------------------------------------------------------------------------
    void AIComponent::Init(FalkonEngine::GameObject* target, float range, std::unique_ptr<IPathfindingStrategy> strategy)
    {
        m_targetPlayer = target;
        m_detectionRange = range;
        m_strategy = std::move(strategy);
    }
    //--------------------------------------------------------------------------------------------------------
    void AIComponent::Update(float dt) 
    {
        if (!m_strategy)
        {
            std::cout << "Strategy not valid" << std::endl;
            return;
        }
        else if (!m_targetPlayer)
        {
            return;
        }

        auto myTransform = GetGameObject()->GetComponent<FalkonEngine::TransformComponent>();
        auto playerTransform = m_targetPlayer->GetComponent<FalkonEngine::TransformComponent>();
        Vector2Df myPos = myTransform->GetWorldPosition();
        Vector2Df playerPos = playerTransform->GetWorldPosition();

        // If the player is within detection range
        if ((playerPos - myPos).GetLength() < m_detectionRange) 
        {
            auto nextPoint = m_strategy->GetNextStep(myPos, playerPos);

            if (nextPoint.GetLength() < 0.001f) 
            {
                return; // The point is too close to zero, ignore it
            }

                GameEvent event;
                event.type = GameEventType::MovementRequested;
                event.sender = this;
                event.direction.x = nextPoint.x;
                event.direction.y = nextPoint.y;

                this->Notify(event); // Sending NPCs
        }
    }
}