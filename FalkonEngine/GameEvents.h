#pragma once

#include "Vector.h"

namespace FalkonEngine
{
    class GameObject;

    enum class GameEventType
    {
        InputDirectionChanged = 0,
        MouseMoved,
        MovementRequested,
        HitVelocityRequest,
        HitRotationRequest,
        PositionChanged,
        StatChanged,
        ActionTriggered,
        ObjectRemoved,
        ObjectSpawned,
        SceneComponentSpawned,
        MovementFinished,
        SubObjectSpawned
    };

    struct GameEvent 
    {
        GameEventType type;
        void* sender;
        int entityID;
        Vector2Df direction;
        float angle;
        int actionID;
        union {
            struct { float x, y, width, height; } area;
            float value;
            GameObject* object;
        };
    };
}