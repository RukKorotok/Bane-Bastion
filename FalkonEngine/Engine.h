#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "Log.h"
#include "SFML/Graphics.hpp"

namespace FalkonEngine {
class Engine {
 public:
  Engine(const Engine& app) = delete;
  Engine& operator=(const Engine&) = delete;

  static Engine* Instance();

  void Run();

 private:
  Engine();
  ~Engine() = default;
};
}  // namespace FalkonEngine