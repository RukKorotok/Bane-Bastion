#pragma once

#include "MoveComponent.h"

namespace BaneAndBastion
{
    class AIMoveComponent : public MoveComponent
    {
    public:
        AIMoveComponent(FalkonEngine::GameObject* gameObject);

        void Update(float deltaTime) override { MoveComponent::Update(deltaTime); }
        void Render() override {}

        void Stop() override;

        void MoveTowards(FalkonEngine::Vector2Df start, FalkonEngine::Vector2Df target);

    protected:
        void CalculateDesiredDir(float dt) override;

    private:
        FalkonEngine::Vector2Df m_startPos{ 0, 0 };
        FalkonEngine::Vector2Df m_targetPos{ 0, 0 };
        bool m_active = false;

    };
}