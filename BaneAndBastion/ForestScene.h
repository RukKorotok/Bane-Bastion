#pragma once

#include <array>
#include <map>
#include <memory>
#include <vector>

#include "EnvironmentObject.h"
#include "GridManager.h"
#include "MusicPlayer.h"
#include "Scene.h"

using namespace FalkonEngine;

namespace BaneAndBastion {

class Player;
class Bane;
class Bastion;
class NPC;

/**
 * @brief Manages the procedural forest environment, hero logic, and chunk-based world updates.
 */
class ForestScene : public Scene {
 public:
  /**
   * @brief Constructs the ForestScene.
   * @param name The unique identifier for the scene.
   */
  ForestScene(const std::string& name);

  /**
   * @brief Destructor, handles cleanup of procedural chunks and managed actors.
   */
  ~ForestScene();

  /**
   * @brief Initializes scene resources, heroes, and the initial grid state.
   */
  void Start() override;

  /**
   * @brief Performs a full scene reset, stopping and restarting the logic.
   */
  void Restart() override;

  /**
   * @brief Stops scene activity and clears the world state.
   */
  void Stop() override;

  /**
   * @brief Resumes scene activity after a pause.
   */
  void Resume() override;

  /**
   * @brief Handles global events, such as entity removal or input triggers.
   * @param event The game event to process.
   */
  void OnNotify(const GameEvent& event) override;

  /**
   * @brief Registers an actor into a specific procedural chunk.
   * @param chunkPos Grid coordinates of the chunk.
   * @param obj Pointer to the actor.
   */
  void AddToChunk(FalkonEngine::Vector2Di chunkPos, FalkonEngine::Actor* obj);

  /**
   * @brief Removes all actors associated with a specific chunk.
   * @param chunkPos Grid coordinates of the chunk to clear.
   */
  void ClearChunk(FalkonEngine::Vector2Di chunkPos);

  /**
   * @brief Gets the grid manager instance for pathfinding and collision checks.
   * @return Pointer to the GridManager.
   */
  GridManager* GetGridManager() const;

  /**
   * @brief Retrieves the primary player character.
   * @return Shared pointer to the player.
   */
  std::shared_ptr<Player> GetPlayer() const;

 private:
  std::shared_ptr<Bastion> m_bastion;        ///< Primary player hero.
  std::shared_ptr<Bane> m_bane;              ///< Tethered secondary hero.
  std::vector<std::shared_ptr<NPC>> m_npcs;  ///< List of active NPCs in the scene.

  GridManager* m_gridManager;  ///< Procedural grid management system.

  /**
   * @brief Mapping of chunk coordinates to their constituent actors.
   */
  std::map<FalkonEngine::Vector2Di, std::vector<Actor*>> m_chunkContent;

  bool m_isPaused = false;  ///< Current pause state of the scene.
};

}  // namespace BaneAndBastion