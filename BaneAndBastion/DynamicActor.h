#pragma once

#include "GameScene.h"
#include "SpriteRendererComponent.h"
#include "MoveComponent.h"
#include "ResourceSystem.h"
#include "Entity.h"
#include "Vector.h"

namespace BaneAndBastion
{
	class DynamicActor : public FalkonEngine::Observer
	{
	public:
		DynamicActor(FalkonEngine::Vector2Df position, std::string name, std::string texture);
		void Destroy();
		FalkonEngine::GameObject* GetGameObject() const;
		void OnNotify(const FalkonEngine::GameEvent& event) override {};

	protected:
		FalkonEngine::GameObject* p_gameObject = nullptr;
		GridManager* p_gridManager = nullptr;
		std::string p_name;

	private:
		bool m_isDestroyed = false;

	};
}