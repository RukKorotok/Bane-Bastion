#include "Wall.h"

#include "GameSettings.h"
#include "ResourceSystem.h"
#include "SpriteColliderComponent.h"
#include "SpriteRendererComponent.h"
#include "pch.h"

namespace BaneAndBastion {
Wall::Wall(FalkonEngine::Vector2Df position)
    : EnvironmentObject(position, "Wall", "wall",
                        FalkonEngine::CollisionCategory::All) {}
}  // namespace BaneAndBastion