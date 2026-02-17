#pragma once
#include "Player.h"
#include "IChainAnchor.h"

namespace BaneAndBastion {
    class Bastion : public Player, public IChainAnchor, public FalkonEngine::Observable
    {
    public:
        Bastion(FalkonEngine::Vector2Df position);

        // IChainAnchor interface
        FalkonEngine::Vector2Df GetAnchorPosition() const override;
        float GetMaxChainDistance() const override;
        void HitAction(FalkonEngine::GameObject& gameObject) override;

        void OnNotify(const FalkonEngine::GameEvent& event) override;
    private:
        float m_maxChainDistance = 200.0f;

    };
}