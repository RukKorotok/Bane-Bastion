#pragma once

#include "MoveComponent.h"

namespace BaneAndBastion
{
    class PlayerMoveComponent : public MoveComponent
    {
    public:
        PlayerMoveComponent(FalkonEngine::GameObject* gameObject);

        void Update(float deltaTime) override { MoveComponent::Update(deltaTime); }
        void Render() override {}

        void Stop() override;

        void SetTargetDirection(FalkonEngine::Vector2Df dir);

    protected:
        void CalculateDesiredDir(float dt) override;

    private:
        FalkonEngine::Vector2Df m_targetDirection{ 0.0f, 0.0f };

    };
}