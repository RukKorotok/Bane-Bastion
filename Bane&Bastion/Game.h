#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <fstream>
#include "SFML/Audio.hpp"
#include "Config.h"
#include "Math.h"
#include "UI.h"
#include "Base.h"
#include "Ball.h"
#include "Block.h"
#include "Sound.h"
#include "InputHandlers.h"
#include "Objects.h"
#include "Factories.h"
#include "SaveSystem.h"
#include "Observer.h"

namespace Bane_Bastion
{
	class RecordItem;

	class GameState
	{
	public:
		GameState() = default;
		~GameState() {};

		virtual void Init(bool afterSave) {};
		virtual void UpdateGame(sf::RenderWindow& window, float deltaTime) {};
		virtual void RefreshMousePosition(float position) {}
		State GetState();
		void AddScore(int score);
		void SetScore(int score);
		int GetScore() const;

	protected:

		State m_state = State::Main;
		int m_score = 500;
		std::shared_ptr<Menu> m_menu = nullptr;
	};

	class GameMenuState : public GameState
	{
	public:
		GameMenuState(State state, int score);
		~GameMenuState() ;
		void UpdateGame(sf::RenderWindow& window, float deltaTime) override;
	};

	class GameStateInRuntime final :  public GameState, public GameObserver
	{
	public:
		GameStateInRuntime(State state, int score);
		~GameStateInRuntime();
		void Init(bool afterSave) override;
		void UpdateGame(sf::RenderWindow& window, float deltaTime) override;
		void AddPrimaryBall(Vector2D ballSize, Vector2D ballPosition, Vector2D ballDirection);
		void RemoveObject(GameObject& object);
		void CreateBall(BallType type, Vector2D position);
		std::shared_ptr<Base> GetBase() const;
		std::vector<std::shared_ptr<Block>> GetBlocks() const;
		std::vector<std::shared_ptr<Ball>> GetPrimaryBalls() const;
		std::vector<std::shared_ptr<Ball>> GetAdditionalBalls() const;
		void ConstructBlock(BlockType type, Vector2D position, Vector2D size, int health);
		void SubtractBlockCount();
		//Handlers
		void DoOnDestracted(std::shared_ptr<GameObservable> observable, ICollidable& other) override;
		void DoOnInteracted(std::shared_ptr<GameObservable> observable) override {}
		void DoOnInput(sf::Keyboard::Key key);

	private:

		void LoadLevel(const std::string& fileName);
		void RemoveObjectByAdress(std::vector<std::shared_ptr<GameObject>> objects, GameObject& adress);
		void AddAdditionalBall(std::shared_ptr<Ball> ball);
		std::shared_ptr<Base> m_base = nullptr;
		std::vector<std::shared_ptr<Ball>> m_primaryBalls;
		std::vector<std::shared_ptr<Ball>> m_additionalBalls;
		std::vector<std::shared_ptr<Block>> m_allBlocks;
		int m_distBlocksCount = 0;
		std::map <BlockType, std::unique_ptr<BlockFactory>> m_BlockFactories;
		std::map <BallType, std::unique_ptr<BallFactory>> m_BallFactories;
	};


	class  Game
	{
	public:

		static Game& GetInstance()
		{
			static Game game;
			return game;
		}
		void Init();
		void SetGameSettings(SettingsMode settingMode);
		void SetGameDifficulty(DifficultyLevel difficulty);
		uint32_t GetSetings();
		DifficultyLevel GetDifficultyLevel();
		std::shared_ptr<GameState> GetCurrentGameState();
		void ExitFromGame();
		void AddGameState(State state, int score, bool afterLoad);
		void RemoveGameState();
		void ResetGame();
		std::vector<std::shared_ptr<RecordItem>> ReadRecordsList();
		void WriteRecordList(const std::vector<std::shared_ptr<RecordItem>> recordList);
		void AddRecordScore(const std::string& name, int newScore);
		void SetWindowRef(sf::RenderWindow* window);
		sf::RenderWindow* GetWindow();
		std::string GetLevelPath(int index);
		std::shared_ptr<GameStateInRuntime> GetRuntimeGameState();
		std::shared_ptr<InputHandler> GetInputHandler();
		int GetLevelCounter();

		void SaveGame();
		void LoadGame();

	private:

		Game();
		~Game() {}

		Game(Game const&) = delete;
		Game& operator = (Game const&) = delete;
		void AddLevelCounter();

		std::string m_LEVELS_PATH = "../Levels";
		std::string m_SAVES_PATH = "../Saves/Save.txt";
		std::vector<std::string> m_levelsPaths;
		DifficultyLevel m_difficulty;
		uint32_t m_settings = 0b0011;
		std::vector<std::shared_ptr<GameState>> m_gameStates;
		std::shared_ptr<GameStateInRuntime> m_runtimeGameState;
		std::vector<std::shared_ptr<RecordItem>> m_recordList;
		const int m_MAX_SCORE = 10;
		sf::RenderWindow* m_window = nullptr;
		std::unique_ptr<GameStateFactory> m_statesFactory;
		std::shared_ptr<InputHandler> m_InputHandler;
		int m_levelCounter = 0;
		Game* m_GameInstance = nullptr;
	};
}