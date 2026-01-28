#pragma once

namespace FalkonEngine
{
    enum class GameEventType
    {
        InputDirectionChanged,
        PositionChanged,
        HealthChanged,
        ActionTriggered
    };

    struct GameEvent {
        GameEventType type;
        void* sender;
        union {
            struct { float x, y; } direction;
            float value;
            int actionID;
        };
    };
}