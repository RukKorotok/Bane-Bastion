#include "pch.h"

#include "Actor.h"
#include "Scene.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

namespace FalkonEngine {

// Actor --- Implementation of the high-level gameplay entity wrapper ---
//--------------------------------------------------------------------------------------------------------
Actor::Actor(Vector2Df position, const std::string& name, std::string texture, CollisionCategory collision)
    : p_name(name), p_collision(collision) {
  // SCENE RESOLUTION: Find where this actor belongs
  auto activeScene = Scene::GetActive();
  if (!activeScene) {
    FE_CORE_ERROR("Actor: Cannot create '" + name + "' because no active scene was found!");
    return;
  }

  auto world = activeScene->GetWorld();
  if (!world) {
    FE_CORE_ERROR("Actor: World is null in active scene while creating '" + name + "'");
    return;
  }

  // GAMEOBJECT CREATION: Instantiate the internal entity within the world
  p_gameObject = world->CreateGameObject<Entity>(name);

  if (!p_gameObject) {
    FE_CORE_ERROR("Actor: Failed to create GameObject for '" + name + "'");
    return;
  }

  // VISUALS: Attach renderer and load texture from the resource system
  auto renderer = p_gameObject->AddComponent<SpriteRendererComponent>();
  auto sharedTexture = FalkonEngine::ResourceSystem::Instance()->GetTextureShared(texture);

  if (sharedTexture) {
    renderer->SetTexture(*sharedTexture);
  } else {
    FE_CORE_ERROR("Actor: Texture '" + texture + "' not found for actor '" + name + "'");
  }

  // TRANSFORMATION: Set the initial world position
  if (auto t = p_gameObject->GetComponent<TransformComponent>()) {
    t->SetWorldPosition(position);
    FE_APP_TRACE("Actor: '" + name + "' created at (" + std::to_string(position.x) + ", " + std::to_string(position.y) +
                 ")");
  } else {
    FE_CORE_WARN("Actor: '" + name + "' created without TransformComponent. Position not set!");
  }
}

//------------------------------------------------------------------------------------------------------------
void Actor::Init() {
  // POTENTIAL EXTENSION: Trigger spawn events or post-construction logic
}

//--------------------------------------------------------------------------------------------------------
void Actor::Destroy() {
  // Prevent double destruction
  if (p_isDestroyed) {
    return;
  }

  auto activeScene = Scene::GetActive();
  if (activeScene && p_gameObject) {
    // WORLD DELEGATION: Let the GameWorld handle the deferred cleanup
    activeScene->GetWorld()->DestroyGameObject(p_gameObject);
  }

  p_isDestroyed = true;
  FE_APP_INFO("Actor: Entity '" + p_name + "' is marked for destruction.");
}

//--------------------------------------------------------------------------------------------------------
FalkonEngine::GameObject* Actor::GetGameObject() const { return p_gameObject; }

//--------------------------------------------------------------------------------------------------------
CollisionCategory Actor::GetCollisionCategory() const { return p_collision; }

//--------------------------------------------------------------------------------------------------------
void Actor::SetCollition(CollisionCategory collision) { p_collision = collision; }

}  // namespace FalkonEngine