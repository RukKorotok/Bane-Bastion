#include "pch.h"

#include "PlayerController.h"
#include "RenderSystem.h"
#include "UIManager.h"

namespace FalkonEngine {

// PlayerController --- Transforming raw hardware input into semantic game actions ---
//------------------------------------------------------------------------------------------------------------
PlayerController::PlayerController() {
  // DEFAULT MAPPINGS: Binding physical keys to virtual axes.
  // This allows logic to check for "Vertical" instead of specific keys.
  BindAxis(sf::Keyboard::W, "Vertical", 1.0f);
  BindAxis(sf::Keyboard::S, "Vertical", -1.0f);
  BindAxis(sf::Keyboard::D, "Horizontal", 1.0f);
  BindAxis(sf::Keyboard::A, "Horizontal", -1.0f);

  // Support for multiple keys per axis (Arrow keys)
  BindAxis(sf::Keyboard::Up, "Vertical", 1.0f);
  BindAxis(sf::Keyboard::Down, "Vertical", -1.0f);
  BindAxis(sf::Keyboard::Right, "Horizontal", 1.0f);
  BindAxis(sf::Keyboard::Left, "Horizontal", -1.0f);

  FE_CORE_INFO("PlayerController: WSAD and Arrow keys bound to axes.");
}

//------------------------------------------------------------------------------------------------------------
void PlayerController::HandleRawEvent(const sf::Event& event) {
  auto& window = RenderSystem::Instance()->GetMainWindow();
  sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

  // UI INTERCEPTION: Check if the mouse is currently over a UI element
  bool isOverUI = UIManager::Instance()->IsPointerOverUI({(float)pixelPos.x, (float)pixelPos.y});

  // 1. MOUSE BUTTON LOGIC (LMB)
  if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
    DispatchMouseEvent(event.mouseButton.button, true);
  } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
    DispatchMouseEvent(event.mouseButton.button, false);
  }

  // 2. DISCRETE KEYBOARD LOGIC (ActionTriggered / ActionReleased)
  if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
    sf::Keyboard::Key keyCode = event.key.code;

    if (m_bindings.count(keyCode)) {
      GameEventType type =
          (event.type == sf::Event::KeyPressed) ? GameEventType::ActionTriggered : GameEventType::ActionReleased;

      for (auto const& action : m_bindings[keyCode]) {
        GameEvent actionEvent;
        actionEvent.type = type;
        actionEvent.sender = this;
        actionEvent.input[action.name] = action.value;
        actionEvent.isConsumedByUI = isOverUI;

        Notify(actionEvent);
      }
    }
  }
}

//------------------------------------------------------------------------------------------------------------
void PlayerController::Update() {
  // 1. AXIS SAMPLING: Accumulate values for continuous movement (e.g., W + Up = 2.0)
  std::map<std::string, float> activeAxes;
  for (auto const& [key, actions] : m_bindings) {
    if (sf::Keyboard::isKeyPressed(key)) {
      for (auto const& action : actions) {
        activeAxes[action.name] += action.value;
      }
    }
  }

  // 2. MOUSE COORDINATE MAPPING: Convert screen pixels to world coordinates
  auto& window = RenderSystem::Instance()->GetMainWindow();
  sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
  sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

  Vector2Df currentMousePos = {worldPos.x, worldPos.y};

  // Thresholding movement to avoid spamming events for sub-pixel jitter
  float dx = currentMousePos.x - m_lastMousePos.x;
  float dy = currentMousePos.y - m_lastMousePos.y;

  if ((dx * dx + dy * dy) > 0.5f) {
    m_lastMousePos = currentMousePos;
    GameEvent mouseEvent;
    mouseEvent.type = FalkonEngine::GameEventType::MouseMoved;
    mouseEvent.sender = this;
    mouseEvent.direction = {currentMousePos.x, currentMousePos.y};
    mouseEvent.isConsumedByUI = UIManager::Instance()->IsPointerOverUI({(float)pixelPos.x, (float)pixelPos.y});
    Notify(mouseEvent);
  }

  // 3. BROADCAST MOVEMENT: Notify observers (like the Player Actor) about current axis state
  if (!activeAxes.empty()) {
    GameEvent event;
    event.type = GameEventType::InputChanged;
    event.sender = this;
    event.input = activeAxes;
    event.isConsumedByUI = UIManager::Instance()->IsPointerOverUI({(float)pixelPos.x, (float)pixelPos.y});
    Notify(event);
  }
}

//------------------------------------------------------------------------------------------------------------
void PlayerController::DispatchMouseEvent(sf::Mouse::Button button, bool isPressed) {
  auto& window = RenderSystem::Instance()->GetMainWindow();
  sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
  bool isOverUI = UIManager::Instance()->IsPointerOverUI({(float)pixelPos.x, (float)pixelPos.y});

  GameEvent event;
  event.type = isPressed ? GameEventType::ActionTriggered : GameEventType::ActionReleased;
  event.sender = this;
  event.direction = m_lastMousePos;
  event.isConsumedByUI = isOverUI;

  if (m_mouseBindings.count(button)) {
    event.input[m_mouseBindings[button]] = 1.0f;
  }

  Notify(event);
}

//------------------------------------------------------------------------------------------------------------
void PlayerController::BindAxis(sf::Keyboard::Key key, const std::string& actionName, float scale) {
  m_bindings[key].push_back({actionName, scale});
}

void PlayerController::BindMouseButton(sf::Mouse::Button button, const std::string& actionName) {
  m_mouseBindings[button] = actionName;
}

Vector2Df PlayerController::GetMouseWorldPos() const { return m_lastMousePos; }

bool PlayerController::IsKeyPressed(sf::Keyboard::Key key) const { return sf::Keyboard::isKeyPressed(key); }

bool PlayerController::IsMouseButtonPressed(sf::Mouse::Button button) const {
  return sf::Mouse::isButtonPressed(button);
}

}  // namespace FalkonEngine