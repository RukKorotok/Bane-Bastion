#pragma once

#include <functional>
#include <vector>

namespace FalkonEngine {

/**
 * @brief Registry for deferred execution of actions.
 * Used to queue tasks (such as scene transitions or object destruction)
 * to be executed at a safe point in the frame lifecycle.
 */
class DeferredActionRegistry {
 public:
  /**
   * @brief Access the global singleton instance.
   * @return Reference to the DeferredActionRegistry.
   */
  static DeferredActionRegistry& Instance();

  /**
   * @brief Queues a function or lambda for execution at the start of the next frame.
   * @param action The function to be deferred.
   */
  void Push(std::function<void()> action);

  /**
   * @brief Executes and clears all accumulated deferred actions.
   * NOTE: Must be called only at a frame-safe boundary to avoid concurrent modification issues.
   */
  void ProcessAll();

 private:
  DeferredActionRegistry() = default;
  ~DeferredActionRegistry() = default;

  std::vector<std::function<void()>> m_actions;  ///< Container for pending tasks.
};

}  // namespace FalkonEngine