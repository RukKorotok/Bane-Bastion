#pragma once

#include "pch.h"
#include <unordered_map>
#include "CollisionCategories.h"

namespace FalkonEngine 
{
    class CollisionRegistry
    {
    public:
        static bool ShouldCollide(FalkonEngine::CollisionCategory a, FalkonEngine::CollisionCategory b) {
            using namespace FalkonEngine;
            // ≈сли категории одинаковые (Player vs Player или Enemy vs Enemy) -> игнорируем
            if (a == b) return false;
            // ¬ остальных случа€х (Player vs Enemy, Player vs Wall и т.д.) -> сталкиваемс€
            return true;
        }
    };
}