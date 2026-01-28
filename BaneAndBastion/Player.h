#pragma once

#include "CameraComponent.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "SpriteColliderComponent.h"
#include "MoveComponent.h"
#include "RenderSystem.h"
#include "InputComponent.h"
#include "Entity.h"

namespace BaneAndBastion
{
	class Player : public FalkonEngine::Observer
	{
	public:
		Player();
		FalkonEngine::GameObject* GetGameObject();
		void OnNotify(const FalkonEngine::GameEvent& event) override;

	private:
		FalkonEngine::GameObject* m_gameObject;

	};
}