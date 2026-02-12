#pragma once

#include "LogParameters.h"
#include "Sink.h"
#include <vector>
#include <memory>
#include <mutex>
#include <iostream>

namespace FalkonEngine 
{
    class Logger 
    {
    public:
        Logger(std::string name) : m_name(std::move(name)) {}

        void AddSink(std::shared_ptr<LogSink> sink) {
            std::lock_guard<std::mutex> lock(m_logMutex);
            m_sinks.push_back(sink);
        }

        void Log(LogLevel level, const std::string& message, const char* file, int line) 
        {
            std::lock_guard<std::mutex> lock(m_logMutex);
            std::string formatted = "[" + std::string(file) + ":" + std::to_string(line) + "] " +
                "[" + m_name + "] " +
                "[" + LogLevelToString(level) + "] " + message;

            if (m_sinks.empty()) {
                std::cout << formatted << std::endl;
            }
            else {
                for (auto& sink : m_sinks) {
                    sink->Log(level, formatted);
                }
            }
        }

    private:
        std::string m_name;
        std::vector<std::shared_ptr<LogSink>> m_sinks;
        std::mutex m_logMutex;
    };
}