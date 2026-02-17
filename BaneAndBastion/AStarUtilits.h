#pragma once

#include "Vector.h"

namespace BaneAndBastion 
{
    struct Node
    {
        FalkonEngine::Vector2Di pos;
        int g, h;
        Node* parent;
        int f() const { return g + h; }
    };

    class AStarUtils
    {
    public:
        static std::vector<FalkonEngine::Vector2Di> FindPath(FalkonEngine::Vector2Di start, FalkonEngine::Vector2Di target);

    };
}