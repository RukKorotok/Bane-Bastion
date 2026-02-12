#pragma once
#include "LoggerRegistry.h"
#include <stdexcept>

#define FE_GET_LOGGER(name) []() { \
    static std::shared_ptr<FalkonEngine::Logger> cachedLogger = \
        FalkonEngine::LoggerRegistry::GetInstance().GetLogger(name); \
    return cachedLogger; \
}()

#define FE_CORE_INFO(msg)  FE_GET_LOGGER("Core")->Log(FalkonEngine::LogLevel::INFO, msg, __FILE__, __LINE__)
#define FE_CORE_WARN(msg)  FE_GET_LOGGER("Core")->Log(FalkonEngine::LogLevel::WARNING, msg, __FILE__, __LINE__)
#define FE_CORE_ERROR(msg) FE_GET_LOGGER("Core")->Log(FalkonEngine::LogLevel::ERROR, msg, __FILE__, __LINE__)

#define FE_APP_INFO(msg)   FE_GET_LOGGER("App")->Log(FalkonEngine::LogLevel::INFO, msg, __FILE__, __LINE__)
#define FE_APP_TRACE(msg)  FE_GET_LOGGER("App")->Log(FalkonEngine::LogLevel::TRACE, msg, __FILE__, __LINE__)

//** If error critical and need stoped now */
#define FE_CORE_ASSERT(condition, msg) if(!(condition)) { \
    FE_CORE_ERROR(std::string("Assertion Failed: ") + msg); \
    throw std::runtime_error(msg); \
}