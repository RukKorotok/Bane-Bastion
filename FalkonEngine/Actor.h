#pragma once

#include "GameObject.h"
#include "Entity.h"
#include "SubScriptionSystem.h"
#include "ResourceSystem.h"
#include "CollisionCategories.h"

namespace FalkonEngine
{
    class Actor : public Observer, public Observable
    {
    public:
        Actor(Vector2Df position, const std::string& name, std::string texture, CollisionCategory collition);
        virtual ~Actor() = default;
        void OnNotify(const FalkonEngine::GameEvent& event) override {};
        void Destroy();

        FalkonEngine::GameObject* GetGameObject() const;
        virtual CollisionCategory GetCollisionCategory() const;

        void SetCollition(CollisionCategory collision);

    protected:
        FalkonEngine::GameObject* p_gameObject = nullptr;
        bool p_isDestroyed = false;
        std::string p_name = "";
        CollisionCategory p_collision = CollisionCategory::None;

    };
}