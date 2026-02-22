#pragma once
#include "Component.h"

namespace FalkonEngine
{
    /**
     * @brief Component responsible for handling damage output and attack timing.
     * * Manages base damage values, damage types, and a cooldown-based hit rate
     * to prevent frame-perfect damage spam.
     */
    class DamageComponent : public Component
    {
    public:
        /**
         * @brief Constructor for the DamageComponent.
         * @param gameObject Pointer to the owner GameObject.
         */
        DamageComponent(GameObject* gameObject);

        virtual ~DamageComponent() = default;

        /**
         * @brief Triggers an attack calculation.
         * * Checks if the component is ready to hit based on cooldown.
         * @return The damage amount if ready, otherwise 0.0f.
         */
        float HitTrigger();

        /** @brief Sets the raw damage value produced by this component. */
        void SetBaseDamage(float amount);

        /** @brief Sets the interval (in seconds) between allowed hits. */
        void SetHitRate(float rate);

        /** @brief Sets a numerical ID representing the type of damage (e.g., Fire, Physical). */
        void SetDamageType(int typeID);

        float GetBaseDamage() const;
        float GetHitRate() const;
        int GetDamageTypeID() const;

        /** @brief Returns true if the cooldown timer has expired. */
        bool GetReadyToHit() const;

        /**
         * @brief Updates the internal cooldown timer.
         * @param deltaTime Time elapsed since the last frame.
         */
        void Update(float deltaTime) override;

        /** @brief Empty implementation; logic-only component. */
        void Render() override;

    private:
        float m_baseDamage = 0.0f;    ///< Amount of damage dealt per hit.
        float m_hitRate = 1.0f;       ///< Cooldown duration in seconds.
        float m_hitCooldown = 0.0f;   ///< Current time remaining until next hit.
        bool m_readyToHit = true;     ///< State flag for the attack readiness.
        int m_damageTypeID = 0;       ///< Identifier for elemental or physical types.

    };
}