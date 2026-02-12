#include "pch.h"

#include "EnvironmentObject.h"
#include "Scene.h"
#include "TransformComponent.h"

namespace BaneAndBastion {
    EnvironmentObject::EnvironmentObject(const std::string& name, float x, float y) 
    {
        auto activeScene = FalkonEngine::Scene::GetActive();
        if (!activeScene)
        {
            FE_CORE_ERROR("EnvironmentObject: Cannot create '" + name + "' because no active scene was found!");
            return;
        }

        auto world = activeScene->GetWorld();
        if (!world)
        {
            FE_CORE_ERROR("EnvironmentObject: World is null in active scene while creating '" + name + "'");
            return;
        }

        m_gameObject = world->CreateGameObject<FalkonEngine::Entity>(name);

        if (!m_gameObject)
        {
            FE_CORE_ERROR("EnvironmentObject: Failed to create GameObject for '" + name + "'");
            return;
        }

        if (auto t = m_gameObject->GetComponent<FalkonEngine::TransformComponent>())
        {
            t->SetWorldPosition(x, y);
            FE_APP_TRACE("EnvironmentObject: '" + name + "' created at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
        }
        else
        {
            FE_CORE_WARN("EnvironmentObject: '" + name + "' created without TransformComponent. Position not set!");
        }
    }
}