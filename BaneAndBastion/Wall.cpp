#include "pch.h"

#include "Wall.h"
#include "SpriteRendererComponent.h"
#include "SpriteColliderComponent.h"
#include "ResourceSystem.h"
#include "GameSettings.h"
#include "GameScene.h"

namespace BaneAndBastion
{
    Wall::Wall(FalkonEngine::Vector2Df position) 
        : EnvironmentObject(position, "Wall", "wall", FalkonEngine::CollisionCategory::All)
    {
    }
}