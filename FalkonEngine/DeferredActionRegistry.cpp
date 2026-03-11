#include "pch.h"
#include "DeferredActionRegistry.h"

namespace FalkonEngine {

//-----------------------------------------------------------------------------------------------------------
DeferredActionRegistry& DeferredActionRegistry::Instance() {
  static DeferredActionRegistry instance;
  return instance;
}

//-----------------------------------------------------------------------------------------------------------
void DeferredActionRegistry::Push(std::function<void()> action) { m_actions.push_back(std::move(action)); }

//-----------------------------------------------------------------------------------------------------------
void DeferredActionRegistry::ProcessAll() {
  if (m_actions.empty()) return;

  // EXECUTION BATCHING: Move pending actions to a local container before execution.
  // This ensures that any new actions pushed during the execution of existing
  // ones are queued for the next frame, preventing infinite loops or
  // iterator invalidation during batch processing.
  std::vector<std::function<void()>> currentBatch = std::move(m_actions);
  m_actions.clear();

  for (auto& action : currentBatch) {
    if (action) {
      action();
    }
  }
}

}  // namespace FalkonEngine