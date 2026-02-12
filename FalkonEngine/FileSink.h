#include "pch.h"
#include "Sink.h"
#include <fstream>
#include <string>
#include <filesystem>

namespace FalkonEngine
{
    class FileSink : public LogSink
    {
    public:
        FileSink(const std::string& fileName)
        {
            std::filesystem::path logPath(fileName);
            if (logPath.has_parent_path())
            {
                std::filesystem::create_directories(logPath.parent_path());
            }

            m_logFile.open(fileName, std::ios::out | std::ios::app);

            if (!m_logFile.is_open())
            {
                fprintf(stderr, "[CRITICAL] Failed to open or create log file: %s\n", fileName.c_str());
            }
            else
            {
                m_logFile << "\n--- New Session Started ---\n";
                m_logFile.flush();
            }
        }

        void Log(LogLevel level, const std::string& message) override
        {
            if (m_logFile.is_open())
            {
                m_logFile << message << std::endl;

                if(level >= LogLevel::ERROR) 
                {
                    m_logFile.flush();
                }
            }
        }

        ~FileSink()
        {
            if (m_logFile.is_open())
            {
                m_logFile.close();
            }
        }

    private:
        std::ofstream m_logFile;
    };
}