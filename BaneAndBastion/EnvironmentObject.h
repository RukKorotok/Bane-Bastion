#pragma once

#include "GameObject.h"
#include "Entity.h"
#include "SubScriptionSystem.h"

namespace BaneAndBastion 
{
    class EnvironmentObject
    {
    public:
        EnvironmentObject(const std::string& name, float x, float y);
        virtual ~EnvironmentObject() = default;

        FalkonEngine::GameObject* GetGameObject() const { return m_gameObject; }

    protected:
        FalkonEngine::GameObject* m_gameObject = nullptr;
    };
}