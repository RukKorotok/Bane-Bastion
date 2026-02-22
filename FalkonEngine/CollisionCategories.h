#pragma once
#include <cstdint>

namespace FalkonEngine
{
    enum CollisionCategory : std::uint8_t
    {
        None = 0,
        Player = 1 << 0, 
        Enemy = 1 << 1, 
        Projectile = 1 << 2,
        Flying = 1 << 3,
        Wall = 1 << 4,
        All = 0xFF
    };
}