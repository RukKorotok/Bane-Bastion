#pragma once
#include <string>

#include "GameWorld.h"
#include "SubScriptionSystem.h"

namespace FalkonEngine {
class Scene : public Observer {
 public:
  Scene(const std::string& name);
  virtual ~Scene();
  static Scene* GetActive();
  GameWorld* GetWorld() const;
  std::string GetName();
  void OnNotify(const GameEvent& event) override;

  virtual void Start() = 0;
  virtual void Restart() = 0;
  virtual void Stop() = 0;

 protected:
  GameWorld* m_world = nullptr;

 private:
  std::string m_name;
};
}  // namespace FalkonEngine