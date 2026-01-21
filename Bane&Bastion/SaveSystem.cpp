#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "SaveSystem.h"
#include "Game.h"
#include "Menu.h"

namespace Bane_Bastion
{
    //SaveManager
    //----------------------------------------------------------------------------------------------------------
    bool SaveManager::Save(const GameStateInRuntime& state, const std::string& path)
    {
        std::ofstream out(path);
        if (!out.is_open()) {
            UI::GetInstance().CreateMenu("InfoMenu", true, "Error! Game not saved");
            return false;
        }
        out << "saveOk" << "\n";
        out << state.GetScore() << "\n";
        out << state.GetPrimaryBalls().size() << "\n";
        for (auto ball : state.GetPrimaryBalls())
        {
            ball->Serialize(out);
        }
        out << state.GetAdditionalBalls().size() << "\n";
        for (auto ball : state.GetAdditionalBalls())
        {
            if (std::dynamic_pointer_cast<PoisonBall>(ball))
            {
                out << BallType::PoisonAction << "\n";
            }
            else if (std::dynamic_pointer_cast<IncreasingBall>(ball))
            {
                out << BallType::IncreasingAction << "\n";
            }
            ball->Serialize(out);
        }
        out << state.GetBlocks().size() << "\n";
        for (auto block : state.GetBlocks())
        {
            block->Serialize(out);
        }
        state.GetBase()->Serialize(out);
        UI::GetInstance().CreateMenu("InfoMenu", true, "Game saved");
        return true;

    }
    //----------------------------------------------------------------------------------------------------------
    bool SaveManager::CheckSave(const std::string& path)
    {
        std::ifstream in(path);
        std::string saveState;
        in >> saveState;
        if (!in.is_open() || saveState != "saveOk")
        {
            return false;
        }
        return true;
    }
    //----------------------------------------------------------------------------------------------------------
    std::shared_ptr<GameState> SaveManager::Load(const std::string& path)
    {
        std::ifstream in(path);
        if (!in.is_open()) 
        {
            return nullptr;
        }

        int i = 0;
        int scores = 0;
        int currentCount = 0;
        int type = 0;
        Vector2D position;
        std::string saveState;

        std::shared_ptr<GameStateInRuntime> state;
        in >> saveState;
        in >> scores;

        state = Game::GetInstance().GetRuntimeGameState();
        state->SetScore(scores);

        in >> currentCount;
        for (i = 0; i < currentCount; i++)
        {
            state->AddPrimaryBall({ BALL_SIZE, BALL_SIZE }, { 0.0f, 0.0f }, { 0.0f, 0.0f });
            state->GetPrimaryBalls().back()->Deserialize(in);
        }
        in >> currentCount;
        for (i = 0; i < currentCount; i++)
        {
            in >> type;
            state->CreateBall(static_cast<BallType>(type), { 0.0f, 0.0f });
            state->GetAdditionalBalls().back()->Deserialize(in);
        }

        in >> currentCount;
        for (i = 0; i < currentCount; i++)
        {
            in >> type >> position.x >> position.y >> scores;
            state->ConstructBlock(static_cast<BlockType>(type), position, { BALL_SIZE * 2, BALL_SIZE }, scores);
            state->GetBlocks().back()->Deserialize(in);
        }

        state->GetBase()->Deserialize(in);

        return state;
    }
    //RecordsManager
    //----------------------------------------------------------------------------------------------------------
    bool RecordsManager::IsEligible(int score)
    {
        auto entries = LoadEntries();
        if (entries.size() < 5) return true;
        return score > entries.back().second;
    }
    //----------------------------------------------------------------------------------------------------------
    void RecordsManager::AddRecord(const std::string& name, int score)
    {
        auto entries = LoadEntries();
        if (entries.size() > 4)
        {
            entries.pop_back();
        }
        entries.push_back({ name, score });

        std::sort(entries.begin(), entries.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second > b.second;
            });

        if (entries.size() > 10) entries.pop_back();

        SaveEntries(entries);
    }
    //----------------------------------------------------------------------------------------------------------
    std::vector<std::pair<std::string, int>> RecordsManager::LoadEntries()
    {
        std::vector<std::pair<std::string, int>> entries;
        std::ifstream file("../Saves/RecordList.txt");
        std::string name;
        int score;
        while (file >> name >> score) {
            entries.push_back({ name, score });
        }
        return entries;
    }
    //-----------------------------------------------------------------------------------------------------------
    std::vector<std::shared_ptr<RecordItem>> RecordsManager::ReadRecordsList()
    {
        std::string name = "";
        std::vector<std::shared_ptr<RecordItem>> list;
        std::ifstream file("../Saves/RecordList.txt");
        int score = 0;

        if (file.is_open())
        {
            while (file >> name >> score)
            {
                list.push_back(std::make_shared<RecordItem>(name, score));
            }
            file.close();
        }
        return list;
    }
    //Private
    //----------------------------------------------------------------------------------------------------------
    void RecordsManager::SaveEntries(const std::vector<std::pair<std::string, int>>& entries)
    {
        std::ofstream file("../Saves/RecordList.txt");
        for (const auto& e : entries) {
            file << e.first << " " << e.second << "\n";
        }
    }
}