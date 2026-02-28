#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

namespace FalkonEngine {

// Тип функции для команды
using UIAction = std::function<void()>;

class UICommandRegistry {
 public:
  // Регистрация команды: связываем ID из JSON с кодом
  static void Bind(const std::string& actionID, UIAction action);

  // Выполнение команды
  static void Execute(const std::string& actionID);

  // Очистка (например, при смене сцены)
  static void Clear();

 private:
  static inline std::unordered_map<std::string, UIAction> m_commands;
};

}  // namespace FalkonEngine