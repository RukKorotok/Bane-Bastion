#pragma once

namespace BaneAndBastion
{
    struct GameSettings
    {
        static constexpr float PixelsPerUnit = 64.0f;

        static constexpr float Gravity = 9.8f;
        const static constexpr int ChunkSize = 16;
    };
}