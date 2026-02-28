#include "pch.h"

#include "GameObject.h"
#include "InputComponent.h"
#include "RenderSystem.h"

namespace FalkonEngine {

// InputComponent --- Captures keyboard and mouse states and notifies observers ---
//-----------------------------------------------------------------------------------------------------------
InputComponent::InputComponent(GameObject* gameObject) : Component(gameObject) {
  FE_APP_TRACE("InputComponent initialized for: " + p_gameObject->GetName());
}

//-----------------------------------------------------------------------------------------------------------
void InputComponent::Update(float deltaTime) {
  float currentVertical = 0.f;
  float currentHorizontal = 0.f;

  // KEYBOARD POLLING: Map WASD keys to movement axes
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    currentVertical += 1.0f;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    currentVertical -= 1.0f;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    currentHorizontal += 1.0f;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    currentHorizontal -= 1.0f;
  }

  // EVENT BROADCAST: Only notify if axis values have changed
  if (currentVertical != m_verticalAxis || currentHorizontal != m_horizontalAxis) {
    m_verticalAxis = currentVertical;
    m_horizontalAxis = currentHorizontal;

    std::string debugMsg = "Input changed on '" + p_gameObject->GetName() + "': H=" + std::to_string(m_horizontalAxis) +
                           ", V=" + std::to_string(m_verticalAxis);

    FE_APP_TRACE(debugMsg);

    try {
      FalkonEngine::GameEvent event;
      event.type = FalkonEngine::GameEventType::InputChanged;
      event.sender = this;
      event.direction = {m_horizontalAxis, m_verticalAxis};

      Notify(event);
    } catch (const std::exception& e) {
      std::string errorMsg = "Exception during Input Notification on '" + p_gameObject->GetName() + "': " + e.what();

      FE_CORE_ERROR(errorMsg);
    }
  }

  // MOUSE TRACKING: Convert pixel screen coordinates to world-space coordinates
  auto& window = RenderSystem::Instance()->GetMainWindow();
  sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

  sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
  FalkonEngine::Vector2Df currentMousePos = {worldPos.x, worldPos.y};

  // PRECISION CHECK: Only notify if the mouse moved beyond a small threshold
  float dx = currentMousePos.x - m_lastMousePos.x;
  float dy = currentMousePos.y - m_lastMousePos.y;

  if ((dx * dx + dy * dy) > 0.5f) {
    m_lastMousePos = currentMousePos;
    try {
      FalkonEngine::GameEvent mouseEvent;
      mouseEvent.type = FalkonEngine::GameEventType::MouseMoved;
      mouseEvent.sender = this;
      mouseEvent.direction = {currentMousePos.x, currentMousePos.y};

      Notify(mouseEvent);
    } catch (const std::exception& e) {
      std::string errorMsg = "Exception during Input Notification on '" + p_gameObject->GetName() + "': " + e.what();

      FE_CORE_ERROR(errorMsg);
    }
  }
}

//-----------------------------------------------------------------------------------------------------------
void InputComponent::Render() {
  // Input component is logic-only and has no visual representation
}

//-----------------------------------------------------------------------------------------------------------
float InputComponent::GetHorizontalAxis() const { return m_horizontalAxis; }

//-----------------------------------------------------------------------------------------------------------
float InputComponent::GetVerticalAxis() const { return m_verticalAxis; }

}  // namespace FalkonEngine