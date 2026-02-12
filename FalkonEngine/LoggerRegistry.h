#pragma once
#include "Logger.h"
#include <unordered_map>
#include <string>

namespace FalkonEngine {
    class LoggerRegistry {
    public:
        static LoggerRegistry& GetInstance() {
            static LoggerRegistry instance;
            return instance;
        }

        std::shared_ptr<Logger> GetLogger(const std::string& name) {
            std::lock_guard<std::mutex> lock(m_registryMutex);
            auto it = m_loggers.find(name);
            if (it != m_loggers.end()) {
                return it->second;
            }

            auto newLogger = std::make_shared<Logger>(name);
            m_loggers[name] = newLogger;
            return newLogger;
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<Logger>> m_loggers;
        std::mutex m_registryMutex;
    };
}