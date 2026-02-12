#pragma once

#include "LogParameters.h"
#include <string>
#include <iostream>

namespace FalkonEngine
{
	class LogSink
	{
	public:
		virtual void Log(LogLevel level, const std::string& message) = 0;
		virtual ~LogSink() = default;

	};
}