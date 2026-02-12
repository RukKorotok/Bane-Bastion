#include "pch.h"

#include "Wall.h"
#include "SpriteRendererComponent.h"
#include "SpriteColliderComponent.h"
#include "ResourceSystem.h"
#include "GameSettings.h"
#include "GameScene.h"

namespace BaneAndBastion {
    Wall::Wall(float x, float y) : EnvironmentObject("StaticWall", x, y) 
    {
        auto renderer = m_gameObject->AddComponent<FalkonEngine::SpriteRendererComponent>();
        auto texture = FalkonEngine::ResourceSystem::Instance()->GetTextureShared("wall");

        float ppu = GameSettings::PixelsPerUnit;

        if (texture) 
        {
            renderer->SetTexture(*texture);
            renderer->SetPixelSize(ppu, ppu);
        }
        else 
        {
            FE_CORE_ERROR("Wall: Texture 'wall' not found! Wall at (" + std::to_string(x) + ", " + std::to_string(y) + ") will be invisible.");
        }

        auto col = m_gameObject->AddComponent<FalkonEngine::SpriteColliderComponent>();

        if (auto* activeScene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive()))
        {
            auto grid = activeScene->GetGridManager();

            if (grid)
            {
                FalkonEngine::Vector2Df pos(x, y);
                FalkonEngine::Vector2Df size = { ppu, ppu };

                grid->OccupyArea(pos, size, m_gameObject->GetID());
            }
            else
            {
                FE_CORE_WARN("Wall: GridManager is null in active scene. Wall at (" + std::to_string(x) + ", " + std::to_string(y) + ") not registered in grid.", x, y);
            }
        }
        else
        {
            FE_CORE_ERROR("Wall: Active scene is not a GameScene! Grid occupation failed for wall at (" + std::to_string(x) + ", " + std::to_string(y) + ")", x, y);
        }
    }
}