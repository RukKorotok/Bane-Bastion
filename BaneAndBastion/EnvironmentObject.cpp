#include "pch.h"

#include "EnvironmentObject.h"
#include "GameScene.h"
#include "TransformComponent.h"
#include "SpriteColliderComponent.h"

namespace BaneAndBastion 
{
    EnvironmentObject::EnvironmentObject(FalkonEngine::Vector2Df position, const std::string& name, const std::string& texture, FalkonEngine::CollisionCategory collision)
        :  FalkonEngine::Actor(position, name, texture, collision)
    {
        auto sprite = p_gameObject->GetComponent<FalkonEngine::SpriteRendererComponent>();
        auto textureResource = FalkonEngine::ResourceSystem::Instance()->GetTextureShared(texture);

        float ppu = GameSettings::PixelsPerUnit;

        if (textureResource)
        {
            sprite->SetTexture(*textureResource);
            sprite->SetPixelSize(int(ppu), int(ppu));
        }
        else
        {
            FE_CORE_ERROR("Wall: Texture 'wall' not found! Wall at (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ") will be invisible.");
        }

        auto col = p_gameObject->AddComponent<FalkonEngine::SpriteColliderComponent>();
    }
}