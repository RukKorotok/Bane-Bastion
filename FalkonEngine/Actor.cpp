#include "pch.h"

#include "Actor.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"

namespace FalkonEngine
{
    //Actor
    //--------------------------------------------------------------------------------------------------------
    Actor::Actor(Vector2Df position, const std::string& name, std::string texture, CollisionCategory collision) : p_name(name), p_collision(collision)
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

        p_gameObject = world->CreateGameObject<FalkonEngine::Entity>(name);

        if (!p_gameObject)
        {
            FE_CORE_ERROR("EnvironmentObject: Failed to create GameObject for '" + name + "'");
            return;
        }

        auto playerRenderer = p_gameObject->AddComponent<FalkonEngine::SpriteRendererComponent>();
        auto sharedTexture = FalkonEngine::ResourceSystem::Instance()->GetTextureShared(texture);

        if (sharedTexture)
        {
            playerRenderer->SetTexture(*sharedTexture);
        }
        else
        {
            FE_CORE_ERROR("DynamicActor: Texture '" + texture + "' not found for actor '" + name + "'");
        }

        if (auto t = p_gameObject->GetComponent<FalkonEngine::TransformComponent>())
        {
            t->SetWorldPosition(position);
            FE_APP_TRACE("EnvironmentObject: '" + name + "' created at (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")");
        }
        else
        {
            FE_CORE_WARN("EnvironmentObject: '" + name + "' created without TransformComponent. Position not set!");
        }

        GameEvent ev;
        ev.type = FalkonEngine::GameEventType::ActorSpawned;
        ev.sender = this;
        ev.senderTypeId = typeid(Actor*).hash_code();
        Notify(ev);
    }
    //--------------------------------------------------------------------------------------------------------
    void Actor::Destroy()
    {
        auto activeScene = FalkonEngine::Scene::GetActive();
        if (activeScene)
        {
            activeScene->GetWorld()->DestroyGameObject(p_gameObject);
        }

        if (p_isDestroyed)
        {
            return;
        }
        p_isDestroyed = true;

        FE_APP_INFO("CharacterManager: Entity '" + p_gameObject->GetName() + "' is destroyed.");
    }
    //--------------------------------------------------------------------------------------------------------
    FalkonEngine::GameObject* Actor::GetGameObject() const 
    { 
        if(p_gameObject)
        { 
            return p_gameObject;
        }
        return nullptr;
    }
    //--------------------------------------------------------------------------------------------------------
    CollisionCategory Actor::GetCollisionCategory() const 
    {
        return p_collision;
    }
    //--------------------------------------------------------------------------------------------------------
    void Actor::SetCollition(CollisionCategory collision)
    {
        p_collision = collision;
    }
}