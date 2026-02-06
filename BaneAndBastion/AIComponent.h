#pragma once

#include "Component.h"
#include "Vector.h"
#include "IPathfindingStrategy.h"
#include <memory>

namespace BaneAndBastion
{
    class GameObject;

    class AIComponent : public Component
    {
    public:
        AIComponent(FalkonEngine::GameObject* gameObject);
        void Init(FalkonEngine::GameObject* target, float range, std::unique_ptr<IPathfindingStrategy> strategy);

        void Update(float deltaTime) override;
        void Render() override {}


    private:
        FalkonEngine::GameObject* m_targetPlayer;
        float m_detectionRange;
        std::unique_ptr<IPathfindingStrategy> m_strategy;

    };
}