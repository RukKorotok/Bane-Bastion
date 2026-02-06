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
        if (texture) {
            renderer->SetTexture(*texture);
        }

        float ppu = GameSettings::PixelsPerUnit;
        renderer->SetPixelSize(ppu, ppu);

        auto col = m_gameObject->AddComponent<FalkonEngine::SpriteColliderComponent>();

        if (auto* activeScene = dynamic_cast<GameScene*>(FalkonEngine::Scene::GetActive()))
        {
            auto grid = activeScene->GetGridManager();

            FalkonEngine::Vector2Df pos(x, y);
            FalkonEngine::Vector2Df size = { ppu, ppu };
            grid->OccupyArea(pos, size, m_gameObject->GetID());
        }
    }
}