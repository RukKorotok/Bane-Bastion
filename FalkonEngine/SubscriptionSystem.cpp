#include "pch.h"

#include "SubscriptionSystem.h"

namespace FalkonEngine {

// Observer --- Handling event reception and subscription cleanup ---
//--------------------------------------------------------------------------------------------------------
Observer::~Observer() {
  FE_CORE_INFO("Observer is being destroyed. Cleaning up subscriptions.");

  // Traverse the list of all subjects this observer is watching
  while (m_head) {
    Details::SubscriptionNode* next = m_head->nextInObs;

    FE_CORE_ASSERT(m_head->subject != nullptr, "SubscriptionNode has null subject during Observer destruction!");

    // Remove the link from the Observable side and free memory
    m_head->subject->_Internal_Detach(m_head);
    delete m_head;

    m_head = next;
  }
}

//--------------------------------------------------------------------------------------------------------
void Observer::UnsubscribeFromAll() {
  Details::SubscriptionNode* curr = m_head;
  while (curr) {
    Details::SubscriptionNode* next = curr->nextInObs;

    // Каждый узел знает, на какой Observable он подписан
    if (curr->subject) {
      curr->subject->_Internal_Detach(curr);
    }

    curr = next;
  }
  m_head = nullptr;  // Список пуст, мы свободны
}

//--------------------------------------------------------------------------------------------------------
void Observer::MarkDead() {
  // Бежим по списку своих подписок
  Details::SubscriptionNode* curr = m_head;
  while (curr) {
    // Мы не удаляем узел! Мы просто ставим "черную метку"
    curr->isDead = true;
    curr = curr->nextInSub;
  }
}

//--------------------------------------------------------------------------------------------------------
void Observer::_Internal_Detach(Details::SubscriptionNode* node) {
  FE_CORE_ASSERT(node != nullptr, "Attempted to detach a null node in Observer.");

  // Standard doubly-linked list node removal
  if (node->prevInObs) {
    node->prevInObs->nextInObs = node->nextInObs;
  }
  if (node->nextInObs) {
    node->nextInObs->prevInObs = node->prevInObs;
  }

  // Update head if the removed node was the first element
  if (m_head == node) {
    m_head = node->nextInObs;
  }
}

// Observable --- Managing subscribers and event broadcasting ---
//--------------------------------------------------------------------------------------------------------
Observable::~Observable() {
  FE_CORE_INFO("Observable is being destroyed. Detaching all observers.");

  // Traverse the list of all observers subscribed to this subject
  while (m_head) {
    Details::SubscriptionNode* next = m_head->nextInSub;

    FE_CORE_ASSERT(m_head->subject != nullptr, "SubscriptionNode has null subject during Observable destruction!");

    // Notify the observer side to remove its link to this subject
    if (m_head->observer) {
      m_head->observer->_Internal_Detach(m_head);
    } else {
      FE_CORE_WARN("SubscriptionNode has null observer during Observable destruction.");
    }

    delete m_head;
    m_head = next;
  }
}

//--------------------------------------------------------------------------------------------------------
void Observable::Subscribe(Observer* obs) {
  if (!obs) {
    FE_CORE_ERROR("Attempted to subscribe a null Observer to an Observable!");
    return;
  }

  // Create a new cross-link node
  auto* node = new Details::SubscriptionNode{this, obs};

  // Insert into Observable's subscriber list
  node->nextInSub = m_head;
  if (m_head) m_head->prevInSub = node;
  m_head = node;

  // Insert into Observer's subscription list
  node->nextInObs = obs->m_head;
  if (obs->m_head) obs->m_head->prevInObs = node;
  obs->m_head = node;

  FE_APP_TRACE("New subscription established between Observable and Observer.");
}

//--------------------------------------------------------------------------------------------------------
void Observable::Notify(const GameEvent& event) {
  // 1. Создаем локальный "снимок" (snapshot) указателей на подписчиков
  // Это гарантирует, что мы не будем зависеть от изменений в связном списке
  std::vector<Observer*> observersToNotify;

  // ВАЖНО: блокируем доступ или просто аккуратно читаем,
  // пока никто не меняет структуру списка (если есть многопоточность)
  Details::SubscriptionNode* curr = m_head;
  while (curr) {
    if (!curr->isDead && curr->observer != nullptr) {
      observersToNotify.push_back(curr->observer);
    }
    curr = curr->nextInSub;
  }

  // 2. Теперь мы работаем с вектором, который НИКТО не удалит из-под нас
  for (auto* obs : observersToNotify) {
    // Дополнительная проверка, на случай если объект умер
    // буквально в процессе рассылки (например, другой Observer удалил его)
    if (obs) {
      obs->OnNotify(event);
    }
  }
}

//--------------------------------------------------------------------------------------------------------
void Observable::_Internal_Detach(Details::SubscriptionNode* node) {
  FE_CORE_ASSERT(node != nullptr, "Attempted to detach a null node in Observable.");

  // Standard doubly-linked list node removal for the subscriber side
  if (node->prevInSub) {
    node->prevInSub->nextInSub = node->nextInSub;
  }
  if (node->nextInSub) {
    node->nextInSub->prevInSub = node->prevInSub;
  }

  // Update head if the removed node was the first element
  if (m_head == node) {
    m_head = node->nextInSub;
  }
}

}  // namespace FalkonEngine