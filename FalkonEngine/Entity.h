#pragma once

#include "pch.h"
#include "GameObject.h"

namespace FalkonEngine {
    class Entity : public GameObject 
    {
    public:
        using GameObject::GameObject;

    protected:
        void HandleEvent(const GameEvent& event) override {}
    };
}