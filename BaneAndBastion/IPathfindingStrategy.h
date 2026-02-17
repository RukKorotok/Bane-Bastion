#pragma once

#include "Vector.h"
#include <vector>
#include "GameScene.h"
namespace BaneAndBastion
{
    class IPathfindingStrategy
    {
    public:
        virtual ~IPathfindingStrategy() = default;

        virtual std::vector<FalkonEngine::Vector2Df> GetPath(FalkonEngine::Vector2Df start, FalkonEngine::Vector2Df target) = 0;
    };
}