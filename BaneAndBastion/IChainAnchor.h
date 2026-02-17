#pragma once
#include "Vector.h"

namespace BaneAndBastion 
{
    class IChainAnchor 
    {
    public:
        virtual ~IChainAnchor() = default;
        virtual FalkonEngine::Vector2Df GetAnchorPosition() const = 0;
        virtual float GetMaxChainDistance() const = 0;
    };
}