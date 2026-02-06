#include "pch.h"

#include "EnvironmentObject.h"
#include "Scene.h"
#include "TransformComponent.h"

namespace BaneAndBastion {
    EnvironmentObject::EnvironmentObject(const std::string& name, float x, float y) 
    {

        auto world = FalkonEngine::Scene::GetActive()->GetWorld();
        m_gameObject = world->CreateGameObject<FalkonEngine::Entity>(name);

        if (auto t = m_gameObject->GetComponent<FalkonEngine::TransformComponent>()) 
        {
            t->SetWorldPosition(x, y);
        }
    }
}