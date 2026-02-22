#pragma once

#include "GameObject.h"
#include "Entity.h"
#include "SubScriptionSystem.h"
#include "Actor.h"

namespace BaneAndBastion 
{
    class EnvironmentObject : public FalkonEngine:: Actor
    {
    public:
        EnvironmentObject(FalkonEngine::Vector2Df position, const std::string& name, const std::string& texture, FalkonEngine::CollisionCategory collision);
        virtual ~EnvironmentObject() = default;
        void OnNotify(const FalkonEngine::GameEvent& event) override {};

    };
}