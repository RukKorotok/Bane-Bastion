#include "pch.h"

#include "Sink.h"

namespace FalkonEngine
{
	class ConsolSink : public LogSink
	{
	public:
		void Log(LogLevel level, const std::string& message) override
		{
			std::cout << LogLevelToString(level) << " " << message << std::endl;
		}

	};
}