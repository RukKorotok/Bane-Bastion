#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Factories.h"

namespace Bane_Bastion
{
    class RecordItem;

	class GameStateInRuntime;

    class SaveManager 
    {
    public:

        static bool Save(const GameStateInRuntime& state, const std::string& path);
        static bool CheckSave(const std::string& path);
        static std::shared_ptr<GameState> Load(const std::string& path);
    };

    class RecordsManager {
    public:
        static RecordsManager& GetInstance() 
        {
            static RecordsManager instance;
            return instance;
        }

        bool IsEligible(int score);
        void AddRecord(const std::string& name, int score);
        std::vector<std::pair<std::string, int>> LoadEntries();
        std::vector<std::shared_ptr<RecordItem>> ReadRecordsList();

    private:
        void SaveEntries(const std::vector<std::pair<std::string, int>>& entries);
    };
}