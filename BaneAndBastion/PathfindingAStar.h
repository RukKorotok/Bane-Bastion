#pragma once

#include "IPathfindingStrategy.h"

namespace BaneAndBastion 
{
    struct Node
    {
        FalkonEngine::Vector2Di pos;
        int g, h;
        Node* parent;
        int f() const { return g + h; }
    };

    class PathfindingAStar : public IPathfindingStrategy
    {
    public:
        FalkonEngine::Vector2Df GetNextStep(FalkonEngine::Vector2Df start, FalkonEngine::Vector2Df target) override;

    private:
        std::vector<FalkonEngine::Vector2Di> PerformAStar(FalkonEngine::Vector2Di start, FalkonEngine::Vector2Di target);
    };
}