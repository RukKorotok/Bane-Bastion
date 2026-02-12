#pragma once

#include <string>

namespace FalkonEngine
{
	enum class LogLevel
	{
        TRACE = 0,
		INFO,
		WARNING,
		ERROR,
        CRITICAL
	};

    inline std::string LogLevelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::TRACE:    return "TRACE";
        case LogLevel::INFO:     return "INFO";
        case LogLevel::WARNING:  return "WARN";
        case LogLevel::ERROR:    return "ERROR";
        case LogLevel::CRITICAL: return "CRIT";
        default:                 return "UNKNOWN";
        }
    }
}