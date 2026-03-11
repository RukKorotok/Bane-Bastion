#include "Tree.h"

#include "pch.h"
#include "GameSettings.h"
#include "ResourceSystem.h"
#include "SpriteColliderComponent.h"
#include "SpriteRendererComponent.h"

namespace BaneAndBastion {
Tree::Tree(FalkonEngine::Vector2Df position)
    : EnvironmentObject(position, "Tree", "tree", FalkonEngine::CollisionCategory::All) {}
}  // namespace BaneAndBastion