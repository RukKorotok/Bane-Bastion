#include "pch.h"

#include "SceneManager.h"
#include "ResourceSystem.h"

namespace FalkonEngine {

// SceneManager
//--------------------------------------------------------------------------------------------------------
SceneManager& SceneManager::Instance() {
  static SceneManager instance;
  return instance;
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::RegisterScene(const std::string& name,
                                 SceneFactory factory) {
  m_factories[name] = factory;
  FE_CORE_INFO("SceneManager: Registered factory for '" + name + "'");
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::OnNotify(const GameEvent& event) {
  switch (event.type) {
    case GameEventType::SceneLoadRequest:
      // В твоем GameEvent можно использовать union или расширить структуру для
      // передачи имени Допустим, мы передаем имя через вспомогательное поле или
      // строку ApplyLoadScene(event.name);
      break;

    case GameEventType::ScenePopRequest:
      ApplyPopScene();
      break;
  }
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::ApplyLoadScene(const std::string& name) {
  auto it = m_factories.find(name);
  if (it != m_factories.end()) {
    ChangeSceneInternal(it->second());
  } else {
    FE_CORE_ERROR("SceneManager: Scene factory not found: " + name);
  }
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::Clear() {
  FE_CORE_INFO("SceneManager: Shutdown cleanup...");
  ChangeSceneInternal(nullptr);
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::ChangeSceneInternal(std::unique_ptr<Scene> newScene) {
  m_isLoading = true;

  // 1. Останавливаем старые сцены
  while (!m_sceneStack.empty()) {
    m_sceneStack.top()->Stop();
    m_sceneStack.pop();
    // ВАЖНО: Мы сразу удаляем сцену здесь.
    // Когда уникальный указатель m_sceneStack.pop() уничтожается,
    // срабатывает деструктор Сцены -> Мира -> Объектов.
  }

  // 2. ТЕПЕРЬ, когда все старые объекты ГАРАНТИРОВАННО мертвы
  // и больше не используют текстуры, мы чистим память.
  ResourceSystem::Instance()->Clear();

  // 3. И только теперь создаем и запускаем новую сцену.
  if (newScene) {
    m_sceneStack.push(std::move(newScene));
    m_sceneStack.top()->Start();  // Здесь загрузятся НОВЫЕ текстуры
  }

  m_isLoading = false;
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::ApplyPushScene(const std::string& name) {
  auto it = m_factories.find(name);
  if (it == m_factories.end()) {
    FE_CORE_ERROR("SceneManager: Cannot push scene. Factory for '" + name +
                  "' not found!");
    return;
  }

  FE_CORE_INFO("SceneManager: Pushing overlay scene '" + name + "'");

  // ВАЖНО: Мы НЕ очищаем ресурсы и НЕ удаляем текущую сцену.
  // Мы просто создаем новую и кладем её на вершину стека.

  std::unique_ptr<Scene> overlayScene = it->second();
  overlayScene->Start();

  m_sceneStack.push(std::move(overlayScene));

  // Теперь в Update() будет обновляться только эта новая сцена.
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::ApplyPopScene() {
  if (m_sceneStack.size() > 1) {
    m_sceneStack.top()->Stop();
    m_sceneStack.pop();
  }
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::Update(float deltaTime) {
  if (!m_sceneStack.empty() && !m_isLoading) {
    // Обновляем только верхнюю сцену (реализация паузы)
    auto* world = m_sceneStack.top()->GetWorld();
    if (world) {
      world->Update(deltaTime);
      world->FixedUpdate(deltaTime);
      world->LateUpdate();
    }
  }
}

//--------------------------------------------------------------------------------------------------------
void SceneManager::Render() {
  if (m_sceneStack.empty()) {
    // Если это вылетит в лог - значит сцена не доехала до стека
    FE_CORE_ERROR("Render: Stack is empty!");
    return;
  }

  if (m_isLoading) {
    // Если экран черный и висит это сообщение - значит Start() завис
    FE_CORE_WARN("Render: Still loading...");
    return;
  }

  auto* activeScene = m_sceneStack.top().get();
  if (activeScene && activeScene->GetWorld()) {
    activeScene->GetWorld()->Render();
  }
}

//--------------------------------------------------------------------------------------------------------
Scene* SceneManager::GetActiveScene() const {
  return m_sceneStack.empty() ? nullptr : m_sceneStack.top().get();
}

}  // namespace FalkonEngine