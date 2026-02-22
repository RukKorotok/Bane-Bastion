#include "pch.h"

#include "DamageComponent.h"
#include "GameObject.h"

namespace FalkonEngine
{
    // DamageComponent implementation
    // --------------------------------------------------------------------------------------------------------
    DamageComponent::DamageComponent(GameObject* gameObject)
        : Component(gameObject)
    {
        FE_APP_TRACE("DamageComponent initialized for: " + p_gameObject->GetName());
    }

    // --------------------------------------------------------------------------------------------------------
    float DamageComponent::HitTrigger()
    {
        // If the component is off cooldown, consume the 'ready' state and return damage
        if (m_readyToHit)
        {
            m_readyToHit = false;
            m_hitCooldown = 0.0f; // Reset timer for the next cooldown cycle
            return m_baseDamage;
        }
        return 0.0f;
    }

    // --------------------------------------------------------------------------------------------------------
    void DamageComponent::SetBaseDamage(float amount)
    {
        m_baseDamage = amount;
    }

    // --------------------------------------------------------------------------------------------------------
    void DamageComponent::SetHitRate(float rate)
    {
        m_hitRate = rate;
    }

    // --------------------------------------------------------------------------------------------------------
    void DamageComponent::SetDamageType(int typeID)
    {
        m_damageTypeID = typeID;
    }

    // --------------------------------------------------------------------------------------------------------
    float DamageComponent::GetBaseDamage() const
    {
        return m_baseDamage;
    }

    // --------------------------------------------------------------------------------------------------------
    float DamageComponent::GetHitRate() const
    {
        return m_hitRate;
    }

    // --------------------------------------------------------------------------------------------------------
    int DamageComponent::GetDamageTypeID() const
    {
        return m_damageTypeID;
    }

    // --------------------------------------------------------------------------------------------------------
    bool DamageComponent::GetReadyToHit() const
    {
        return m_readyToHit;
    }

    // --------------------------------------------------------------------------------------------------------
    void DamageComponent::Update(float deltaTime)
    {
        // Handle cooldown timer if the component is currently in a waiting state
        if (!m_readyToHit)
        {
            if (m_hitCooldown < m_hitRate)
            {
                m_hitCooldown += deltaTime;
            }
            else
            {
                // Set to ready once the elapsed time exceeds the hit rate interval
                m_readyToHit = true;
                m_hitCooldown = 0.0f;
            }
        }
    }

    // --------------------------------------------------------------------------------------------------------
    void DamageComponent::Render() {}
}