#include "Sink.h"
#include "pch.h"

namespace FalkonEngine {
class ConsolSink : public LogSink {
 public:
  void Log(LogLevel level, const std::string& message) override {
    std::cout << LogLevelToString(level) << " " << message << std::endl;
  }
};
}  // namespace FalkonEngine