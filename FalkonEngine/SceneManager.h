#pragma once

#include <functional>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "Scene.h"
#include "SubScriptionSystem.h"

namespace FalkonEngine {

using SceneFactory = std::function<std::unique_ptr<Scene>()>;

class SceneManager : public Observer {
 public:
  static SceneManager& Instance();

  // Регистрация "чертежа" сцены
  void RegisterScene(const std::string& name, SceneFactory factory);

  // Обработка событий (SceneLoadRequest, ScenePopRequest и т.д.)
  void OnNotify(const GameEvent& event) override;

  // Жизненный цикл (вызывается из Engine::Run)
  void Update(float deltaTime);
  void Render();

  Scene* GetActiveScene() const;
  void ApplyLoadScene(const std::string& name);
  void Clear();

 private:
  SceneManager() = default;
  ~SceneManager() = default;

  // Запрещаем копирование
  SceneManager(const SceneManager&) = delete;
  SceneManager& operator=(const SceneManager&) = delete;

  void ChangeSceneInternal(std::unique_ptr<Scene> newScene);

  void ApplyPushScene(const std::string& name);
  void ApplyPopScene();

  std::map<std::string, SceneFactory> m_factories;
  std::stack<std::unique_ptr<Scene>> m_sceneStack;
  bool m_isLoading = false;
};

}  // namespace FalkonEngine