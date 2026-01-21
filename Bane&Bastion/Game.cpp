#include "Game.h"
#include "Menu.h"
#include <bitset>


namespace Bane_Bastion
{
	//GameState
	//-----------------------------------------------------------------------------------------------------------
	State GameState::GetState()
	{
		return m_state;
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameState::AddScore(int score)
	{
		m_score = m_score + score;
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameState::SetScore(int score)
	{
		m_score = score;
	}
	//-----------------------------------------------------------------------------------------------------------
	int GameState::GetScore() const
	{
		return m_score;
	}
	//GameMenuState
	//-----------------------------------------------------------------------------------------------------------
	GameMenuState::GameMenuState(State state, int score)
	{
		switch (state)
		{
		case State::Main:
		{
			m_menu = UI::GetInstance().CreateMenu("Main", false, "");
			break;
		}
		case State::ResumeGame:
		{
			m_menu = UI::GetInstance().CreateMenu("PauseMenu", false, "");
			break;
		}
		case State::GameOver:
		{
			m_menu = UI::GetInstance().CreateMenu("GameOverMenu", false, "");

			break;
		}
		case State::Win:
		{
			m_menu = UI::GetInstance().CreateMenu("WinMenu", false, "");
			break;
		}
		}
		m_state = state;
		m_score = score;
	}
	//-----------------------------------------------------------------------------------------------------------
	GameMenuState::~GameMenuState()
	{
		UI::GetInstance().RemoveMenu(*m_menu);
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameMenuState::UpdateGame(sf::RenderWindow& window, float deltaTime)
	{
		window.clear();
		UI::GetInstance().DrawMenu(window);
		window.display();
	}

	//GameStateInRuntime
	//-----------------------------------------------------------------------------------------------------------
	GameStateInRuntime::GameStateInRuntime(State state, int score)
	{

		m_BlockFactories[BlockType::Normal] = std::make_unique<DistractedBlockFactory>();
		m_BlockFactories[BlockType::ScoreUp] = std::make_unique<BlockScoreUpFactory>();
		m_BlockFactories[BlockType::Poison] = std::make_unique<PoisonBlockFactory>();
		m_BlockFactories[BlockType::Disorient] = std::make_unique<DisorientBlockFactory>();
		m_BlockFactories[BlockType::Wall] = std::make_unique<WallBlockFactory>();
		m_BallFactories[BallType::Primary] = std::make_unique<PrimaryBallFactory>();
		m_BallFactories[BallType::PoisonAction] = std::make_unique<PoisonBallFactory>();
		m_BallFactories[BallType::IncreasingAction] = std::make_unique<IncreasingBallFactory>();

		m_score = score;
		m_base = std::make_shared<Base>();
		Game::GetInstance().GetInputHandler()->AddObserver(std::dynamic_pointer_cast<InputObserver>(m_base));
	}
	//-----------------------------------------------------------------------------------------------------------
	GameStateInRuntime::~GameStateInRuntime()
	{
		m_primaryBalls.clear();
		m_base.reset();
		m_additionalBalls.clear();
		m_allBlocks.clear();
	}
	//-----------------------------------------------------------------------------------------------------------
	void Bane_Bastion::GameStateInRuntime::Init(bool afterSave)
	{
		Vector2D ballSize = { BALL_SIZE, BALL_SIZE };
		Vector2D ballPosition = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };
		Vector2D ballDirection = { 0.0f, -1.0f };
		if (!afterSave)
		{
			LoadLevel(Game::GetInstance().GetLevelPath(Game::GetInstance().GetLevelCounter()));
		}
		AddPrimaryBall(ballSize, ballPosition, ballDirection);
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::UpdateGame(sf::RenderWindow& window, float deltaTime)
	{
		window.clear();

		for (const auto block : m_allBlocks)
		{
			if(block)
			{
				block->Visualize(window);
				block->UpdateBlock(deltaTime);
			}
		}
		for (const auto ball : m_primaryBalls)
		{
			if (ball)
			{
				ball->UpdateBall(deltaTime, window);
			}
		}

		for (const auto ball : m_additionalBalls)
		{
			if (ball)
			{
				ball->UpdateBall(deltaTime, window);
			}
		}
		m_base->UpdateBase(deltaTime, window);
		UI::GetInstance().DrawGameUI(Game::GetInstance(), window);
		window.display();
	}
	//------------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::AddPrimaryBall(Vector2D ballSize, Vector2D ballPosition, Vector2D ballDirection)
	{
		m_primaryBalls.push_back(m_BallFactories.at(BallType::Primary)->CreateBall(ballPosition, ballSize, ballDirection, sf::Color::White));
		m_primaryBalls.back()->SetSelfObservable(m_primaryBalls.back());
		m_primaryBalls.back()->AddObserver(GetObserverRef());
		m_primaryBalls.back()->AddObserver(Audio::GetInstance().GetWeakObserver());
	}
	//------------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::RemoveObject(GameObject& object)
	{
		int i = 0;
		GameObject* objectAdress = &object;
		for (i = 0; i < m_additionalBalls.size(); i++)
		{
			if (m_additionalBalls[i].get() == objectAdress)
			{
				m_additionalBalls.erase(m_additionalBalls.begin() + i);
				return;
			}
		}

		for (i = 0; i < m_primaryBalls.size(); i++)
		{
			if (m_primaryBalls[i].get() == objectAdress)
			{
				m_primaryBalls.erase(m_primaryBalls.begin() + i);

				if (m_primaryBalls.empty())
				{
					Game::GetInstance().AddGameState(GameOver, Game::GetInstance().GetCurrentGameState()->GetScore(), false);
				}
				return;
			}
		}

		for (i = 0; i < m_allBlocks.size(); i++)
		{
			if (m_allBlocks[i].get() == objectAdress)
			{
				m_allBlocks.erase(m_allBlocks.begin() + i);
			}
		}
	}
	//------------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::CreateBall(BallType type, Vector2D position)
	{
		sf::Color color;
		switch (type)
		{
		case BallType::PoisonAction :
		{
			color = sf::Color::Green;
			break;
		}
		case BallType::IncreasingAction:
		{
			color = sf::Color::Blue;
			break;
		}
		}
		Vector2D size = { BALL_SIZE, BALL_SIZE };
		Vector2D vectorSpeed = { 0.0f, 1.0f };
		AddAdditionalBall(m_BallFactories.at(type)->CreateBall(position, size, vectorSpeed, color));
	}
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<Base> GameStateInRuntime::GetBase() const
	{
		return m_base;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::vector<std::shared_ptr<Block>> GameStateInRuntime::GetBlocks() const
	{
		return m_allBlocks;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::vector<std::shared_ptr<Ball>> GameStateInRuntime::GetPrimaryBalls() const
	{
		return m_primaryBalls;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::vector<std::shared_ptr<Ball>> GameStateInRuntime::GetAdditionalBalls() const
	{
		return m_additionalBalls;
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::ConstructBlock(BlockType type, Vector2D position, Vector2D size, int health)
	{
		m_allBlocks.emplace_back(m_BlockFactories.at(type)->CreateBlock(position, size, health) );
		if (type != BlockType::Wall)
		{
			m_allBlocks.back()->SetSelfObservable(m_allBlocks.back());
			m_allBlocks.back()->AddObserver(GameObserver::GetObserverRef());
			m_distBlocksCount++;
		}
		m_allBlocks.back()->AddObserver(Audio::GetInstance().GetWeakObserver());
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::SubtractBlockCount()
	{
		m_distBlocksCount--;
		if (m_distBlocksCount == 0)
		{
			Game::GetInstance().AddGameState(Win, m_score, false);
		}
	}
	//Handlers
	//-----------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::DoOnDestracted(std::shared_ptr<GameObservable> observable, ICollidable& other)
	{
		Vector2D ballSize = { BALL_SIZE, BALL_SIZE };
		Vector2D ballDerection = { 0.0f, 1.0f };
		bool random_bool = false;
		if (auto block = std::dynamic_pointer_cast<DistractedBlock>(observable); block)
		{
			if (std::dynamic_pointer_cast<BlockScoreUp>(observable))
			{
				AddScore(block->GetMaxHealth() * 10);
			}
			else
			{

				if (std::dynamic_pointer_cast<PoisonBlock>(observable))
				{
					CreateBall(BallType::PoisonAction, block->GetPosition());
				}
				else if (std::dynamic_pointer_cast<DisorientBlock>(observable))
				{
					CreateBall(BallType::IncreasingAction, block->GetPosition());
				}
				AddScore(block->GetMaxHealth());
			}
			SubtractBlockCount();
			if (random_bool = (std::rand() % 100) < 25) 
			{
				AddPrimaryBall(ballSize, { block->GetPosition().x, block->GetPosition().y + BALL_SIZE }, ballDerection);
			}
		}
		RemoveObject(*std::dynamic_pointer_cast<GameObject>(observable));
	}
	//------------------------------------------------------------------------------------------------------------
	void Bane_Bastion::GameStateInRuntime::DoOnInput(sf::Keyboard::Key key)
	{
		if (key == PAUSE && Game::GetInstance().GetCurrentGameState().get() == this)
		{
			Game::GetInstance().AddGameState(State::ResumeGame, m_score, false);
		}
	}
	//------------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::LoadLevel(const std::string& fileName)
	{
		BlockType blockType;
		const Vector2D size = { BASE_SEGMENT_SIZE * 2, BASE_SEGMENT_SIZE };
		std::ifstream file(fileName);
		std::string line;
		int row = 0;
		Vector2D position;
		char symbol;
		int numberSymbol = 0;
		//Open file
		if (!file.is_open())
		{
			std::cerr << "Error, not open level: " << fileName << std::endl;
			return;
		}
		//Get line
		while (std::getline(file, line))
		{
			for (int c = 0; c < line.length(); c++)
			{
				symbol = line[c];
				//Check symbols
				if (symbol != '#')
				{
					position = { c * BASE_SEGMENT_SIZE * 2 + BASE_SEGMENT_SIZE, row * BASE_SEGMENT_SIZE + BASE_SEGMENT_SIZE * 0.5f };

					if  (symbol == 'M')
					{
						ConstructBlock(BlockType::Wall, position, size, 1);
						continue;
					}
					else if (symbol == '$')
					{
						ConstructBlock(BlockType::ScoreUp, position, size, 1);
					}
					else if (symbol == 'P')
					{
						ConstructBlock(BlockType::Poison, position, size, 1);
					}
					else if (symbol == 'D')
					{
						ConstructBlock(BlockType::Disorient, position, size, 1);
					}

					numberSymbol = symbol - '0';
					if (numberSymbol > 0 && numberSymbol < 10)
					{
						ConstructBlock(BlockType::Normal, position, size, numberSymbol);
					}
				}
			}
			row++;
		}
		// Close file
		file.close();
	}
	//-----------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::RemoveObjectByAdress(std::vector<std::shared_ptr<GameObject>> objects, GameObject& adress)
	{
		int i = 0;
		for (i = 0; i < objects.size(); i++)
		{
			if (objects[i].get() == &adress)
			{
				objects.erase(objects.begin() + i);
				return;
			}
		}
	}
	//------------------------------------------------------------------------------------------------------------
	void GameStateInRuntime::AddAdditionalBall(std::shared_ptr<Ball> ball)
	{
		m_additionalBalls.push_back(ball);
		ball->SetSelfObservable(ball);
		if (std::dynamic_pointer_cast<IncreasingBall>(ball))
		{
			ball->AddObserver(std::static_pointer_cast<GameObserver>(m_base));
			ball->AddObserver(GameObserver::GetObserverRef());
		}
		else if (std::dynamic_pointer_cast<PoisonBall>(ball))
		{
			for (std::shared_ptr<Ball> item : m_primaryBalls)
			{
				for (const auto item : m_primaryBalls)
				{
					ball->AddObserver(item->GetObserverRef());
				}
				ball->AddObserver(GameObserver::GetObserverRef());
			}
		}
		m_additionalBalls.back()->AddObserver(Audio::GetInstance().GetWeakObserver());
	}
	//Game
	//-----------------------------------------------------------------------------------------------------------
	void Game::SetGameSettings(SettingsMode settingMode)
	{
		// Set game settings
		if (m_settings & static_cast<uint32_t>(settingMode))
		{
			m_settings &= ~static_cast<uint32_t>(settingMode);
		}
		else
		{
			m_settings |= static_cast<uint32_t>(settingMode);
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::SetGameDifficulty(DifficultyLevel difficulty)
	{
		m_difficulty = difficulty;
	}
	//-----------------------------------------------------------------------------------------------------------
	uint32_t Game::GetSetings()
	{
		return m_settings;
	}
	//-----------------------------------------------------------------------------------------------------------
	DifficultyLevel Game::GetDifficultyLevel()
	{
		return m_difficulty;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<GameState> Game::GetCurrentGameState()
	{
		if (!m_gameStates.empty())
		{
			return m_gameStates[m_gameStates.size() - 1];
		}
		return nullptr;
	}

	//-----------------------------------------------------------------------------------------------------------
	void Game::ExitFromGame()
	{
		m_window->close();
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::AddGameState(State state, int score, bool afterLoad)
	{
		if (!m_statesFactory)
		{
			m_statesFactory = std::make_unique<GameStateFactory>();
		}
		m_gameStates.push_back(m_statesFactory->CreateState(state, score) );
		if (state == State::GameInProgress)
		{
			m_runtimeGameState = std::dynamic_pointer_cast<GameStateInRuntime>(m_gameStates.back());
			GetInputHandler()->AddObserver(m_runtimeGameState);
		}

		if (state == State::Win)
		{
			AddLevelCounter();
		}
		m_gameStates.back()->Init(afterLoad);
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::RemoveGameState()
	{
		if (!m_gameStates.empty())
		{
			m_gameStates.pop_back();
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::ResetGame()
	{
		m_gameStates.clear();
		AddGameState(State::Main, 0, false);
	}
	//-----------------------------------------------------------------------------------------------------------
	std::vector<std::shared_ptr<RecordItem>> Game::ReadRecordsList()
	{
		std::string name = "";
		std::vector<std::shared_ptr<RecordItem>> list;
		std::ifstream file("../Saves/RecordList.txt");
		int score = 0;

		if (file.is_open())
		{
			while (file >> name >> score)
			{
				list.push_back(std::make_shared<RecordItem>(name, score) );
			}
			file.close();
		}
		return list;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::WriteRecordList(const std::vector<std::shared_ptr<RecordItem>> recordList)
	{
		std::ofstream file("../Saves/RecordList.txt");
		if (file.is_open())
		{
			for (const auto& entry : recordList) 
			{
				file << entry->GetString() << " " << entry->GetString() << std::endl;
			}
			file.close();
		}
		else 
		{
			std::cerr << "File dont open, ERROR!!!" << std::endl;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::AddRecordScore(const std::string& name, int newScore)
	{
		std::vector<std::shared_ptr<RecordItem>> scores = ReadRecordsList();
		std::shared_ptr<RecordItem> insertValue;
		std::vector<std::shared_ptr<RecordItem>> tempList = scores;
		scores.push_back(std::make_shared<RecordItem>(name, newScore) );
		int i = 0;
		int j = 0;

		for (i = 0; i < scores.size(); ++i)
		{
			insertValue = tempList[i];
			j = i - 1;

			while (j >= 0 && tempList[j]->GetScore() < insertValue->GetScore() )
			{
				tempList[j + 1] = tempList[j];
				j = j - 1;
			}
			tempList[j + 1] = insertValue;
		}
		scores = tempList;

		if (scores.size() > m_MAX_SCORE)
		{
			scores.resize(m_MAX_SCORE);
		}
		WriteRecordList(scores);
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::SetWindowRef(sf::RenderWindow* window)
	{
		m_window = window;
	}
	//-----------------------------------------------------------------------------------------------------------
	sf::RenderWindow* Game::GetWindow()
	{
		return m_window;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::string Game::GetLevelPath(int index)
	{
		if (m_levelsPaths.size() > index)
		{
			return m_levelsPaths[index];
		}
		std::cerr << "Level not found: " << m_LEVELS_PATH << std::endl;
		return "";
	}
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<GameStateInRuntime> Game::GetRuntimeGameState()
	{
		return m_runtimeGameState;
	}
	//-----------------------------------------------------------------------------------------------------------
	std::shared_ptr<InputHandler> Game::GetInputHandler()
	{
		return m_InputHandler;
	}
	//-----------------------------------------------------------------------------------------------------------
	int Game::GetLevelCounter()
	{
		return m_levelCounter;
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::SaveGame()
	{
		std::shared_ptr<GameStateInRuntime> state = GetRuntimeGameState();
		SaveManager::Save(*state, m_SAVES_PATH);
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::LoadGame()
	{
		AddGameState(State::GameInProgress, 0, true);
		SaveManager::Load(m_SAVES_PATH);
	}
	//-----------------------------------------------------------------------------------------------------------
	Game::Game()
	{
				m_InputHandler = std::make_shared<InputHandler>();
		//Find all paths for levels files
		try
		{
			std::filesystem::path folderPath(m_LEVELS_PATH);

			if (std::filesystem::exists(folderPath) && std::filesystem::is_directory(folderPath))
			{
				for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(folderPath))
				{
					if (std::filesystem::is_regular_file(entry.status()))
					{
						// Added path in paths list
						m_levelsPaths.push_back(entry.path().string());
					}
				}
			}
			else
			{
				std::cerr << "Folder not found: " << m_LEVELS_PATH << std::endl;
			}
		}
		catch (const std::filesystem::filesystem_error& error)
		{
			std::cerr << "File system error: " << error.what() << std::endl;
		}
	}
	//-----------------------------------------------------------------------------------------------------------
	void Game::AddLevelCounter()
	{
		if (m_levelCounter < m_levelsPaths.size() - 1)
		{
			m_levelCounter++;
		}
		else
		{
			m_levelCounter = 0;
		}
	}
	void Game::Init()
	{
		AddGameState(State::Main, 0, false);
	}
}
