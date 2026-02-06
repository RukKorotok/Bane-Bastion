#pragma once

namespace FalkonEngine
{
    class GameObject;

    enum class GameEventType
    {
        InputDirectionChanged = 0,
        MovementRequested,
        PositionChanged,
        HealthChanged,
        ActionTriggered,
        ObjectRemoved,
        MovementFinished,
        SubObjectSpawned
    };

    struct GameEvent 
    {
        GameEventType type;
        void* sender;
        int entityID;
        union {
            struct { float x, y; } direction;
            struct { float x, y, width, height; } area;
            float value;
            int actionID;
            GameObject* object;
        };
    };
}