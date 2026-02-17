#pragma once

#include <iostream>
#include <array>
#include "Scene.h"
#include "MusicPlayer.h"
#include "GridManager.h"
#include "EnvironmentObject.h"

using namespace FalkonEngine;

namespace BaneAndBastion
{
	class Player;
	class Bane;
	class Bastion;
	class NPC;

	class GameScene : public Scene
	{
	public:
		GameScene(const std::string& name);
		~GameScene();
		void Start() override;
		void Restart() override;
		void Stop() override;
		void OnNotify(const GameEvent& event) override;

		void AddToChunk(FalkonEngine::Vector2Di chunkPos, BaneAndBastion::EnvironmentObject* obj);
		void ClearChunk(FalkonEngine::Vector2Di chunkPos);

		GridManager* GetGridManager() const;
		std::shared_ptr<Player> GetPlayer() const;
	private:
		std::shared_ptr<Bastion> m_bastion;
		std::shared_ptr<Bane> m_bane;
		std::shared_ptr<NPC> m_npc;
		GridManager* m_gridManager;
		std::map<FalkonEngine::Vector2Di, std::vector<EnvironmentObject*>> m_chunkContent;
	};
}