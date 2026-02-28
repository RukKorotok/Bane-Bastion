#include "pch.h"

#include <iostream>
#include "Component.h"

namespace FalkonEngine {

// Component --- Foundation for the Entity-Component System (ECS) ---
//-----------------------------------------------------------------------------------------------------------
Component::Component(GameObject* gameObject) : p_gameObject(gameObject) {
  // VALIDATION: Ensure every component is strictly tied to a valid owner
  FE_CORE_ASSERT(p_gameObject != nullptr, "Component created without a valid GameObject pointer!");
}

//-----------------------------------------------------------------------------------------------------------
Component::~Component() {
  // TRACING: Log the deletion of the component for memory tracking/debugging
  FE_APP_TRACE("Deleted component: " + std::to_string(reinterpret_cast<uintptr_t>(this)));
}

//-----------------------------------------------------------------------------------------------------------
GameObject* Component::GetGameObject() {
  // SAFETY CHECK: Ensure the owner reference hasn't been corrupted or lost
  FE_CORE_ASSERT(p_gameObject != nullptr, "Component's owner GameObject is null during GetGameObject() call!");

  return p_gameObject;
}

}  // namespace FalkonEngine