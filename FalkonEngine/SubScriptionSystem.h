#pragma once

#include "GameEvents.h"

namespace FalkonEngine {

class Observable;
class Observer;

namespace Details {
/**
 * @brief Internal node managing the many-to-many relationship between subjects and observers.
 * Uses a quad-linked list structure to allow O(1) removal from both the
 * Observable's subscriber list and the Observer's subscription list.
 */
struct SubscriptionNode {
  class Observable* subject;  ///< The object being watched.
  class Observer* observer;   ///< The object listening for events.
  bool isDead = false;

  // Links for the Observable's list (all observers subscribed to one subject)
  SubscriptionNode* nextInSub = nullptr;
  SubscriptionNode* prevInSub = nullptr;

  // Links for the Observer's list (all subjects one observer is watching)
  SubscriptionNode* nextInObs = nullptr;
  SubscriptionNode* prevInObs = nullptr;
};
}  // namespace Details

/**
 * @brief Base interface for objects that receive game events.
 * Any class inheriting from Observer can subscribe to an Observable subject.
 */
class Observer {
 public:
  /**
   * @brief Destructor. Automatically unsubscribes from all subjects to prevent dangling pointers.
   */
  virtual ~Observer();

  void UnsubscribeFromAll();

  void MarkDead();
  /**
   * @brief Pure virtual callback triggered by the Observable.
   * @param event The event packet containing message data.
   */
  virtual void OnNotify(const GameEvent& event) = 0;

 private:
  friend class Observable;

  /**
   * @brief Detaches a specific subscription node from the observer's internal list.
   * @param n Pointer to the node to be removed.
   */
  void _Internal_Detach(Details::SubscriptionNode* n);

  Details::SubscriptionNode* m_head = nullptr;  ///< Head of the linked list of active subscriptions.
};

/**
 * @brief Base class for objects that emit events.
 * Manages a list of subscribers and broadcasts GameEvents to them.
 */
class Observable {
 public:
  /**
   * @brief Destructor. Automatically notifies all observers that the subject is being destroyed.
   */
  virtual ~Observable();

  /**
   * @brief Registers an observer to receive notifications from this subject.
   * @param obs Pointer to the observer to be registered.
   */
  void Subscribe(Observer* obs);

  /**
   * @brief Broadcasts an event to all currently subscribed observers.
   * @param event The event data to be sent.
   */
  void Notify(const GameEvent& event);

 private:
  friend class Observer;

  /**
   * @brief Detaches a specific subscription node from the subject's internal list.
   * @param node Pointer to the node to be removed.
   */
  void _Internal_Detach(Details::SubscriptionNode* node);

  Details::SubscriptionNode* m_head = nullptr;  ///< Head of the linked list of registered observers.
};

}  // namespace FalkonEngine