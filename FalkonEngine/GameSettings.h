#pragma once

namespace FalkonEngine
{
    struct GameSettings
    {
        static constexpr float PixelsPerUnit = 64.0f;

        static constexpr float Gravity = 9.8f;
        static constexpr int ChunkSize = 16; 
    };
}