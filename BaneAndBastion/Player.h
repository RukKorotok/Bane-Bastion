#pragma once

#include "CameraComponent.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "RenderSystem.h"
#include "InputComponent.h"
#include "GameObject.h"

namespace BaneAndBastion
{
	class Player
	{
	public:
		Player();
		FalkonEngine::GameObject* GetGameObject();
	private:
		FalkonEngine::GameObject* gameObject;
	};
}