#pragma once

#include "AIComponent.h"
#include "Character.h"
#include "AIMoveComponent.h"

namespace BaneAndBastion
{
    /**
     * @brief Specialized Character class for non-player entities.
     * * Integrates AI-driven decision making and movement logic.
     * Inherits core combat and physical properties from the Character base class.
     */
    class NPC : public Character
    {
    public:
        /**
         * @brief Constructs an NPC at a specific world coordinate.
         * @param position Initial 2D coordinates for the NPC's transform.
         */
        NPC(FalkonEngine::Vector2Df position);

        /**
         * @brief Defines the behavior when this NPC attacks or interacts with another object.
         * Overrides the base Character hit logic (e.g., applying damage or triggering animations).
         * @param gameObject The target object being hit.
         */
        void HitAction(FalkonEngine::GameObject& gameObject) override;

        /**
         * @brief Observer pattern implementation to handle global or local game events.
         * Processes events such as "MovementFinished" from AIMoveComponent or "TargetSpotted".
         * @param event The dispatched game event to be processed.
         */
        void OnNotify(const FalkonEngine::GameEvent& event) override;

    };
}