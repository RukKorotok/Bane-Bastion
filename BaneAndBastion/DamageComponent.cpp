#include "pch.h"

#include "DamageComponent.h"
#include "GameObject.h"

namespace FalkonEngine
{
    //DamageComponent
    //--------------------------------------------------------------------------------------------------------
    DamageComponent::DamageComponent(GameObject* gameObject)
        : Component(gameObject)
    {
        FE_APP_TRACE("DamageComponent initialized for: " + p_gameObject->GetName());
    }
    //--------------------------------------------------------------------------------------------------------
    float DamageComponent::HitTrigger()
    {
        if (m_readyToHit) 
        {
            m_readyToHit = false;
            return m_baseDamage;
        }
        return 0.0f;
    }
    //--------------------------------------------------------------------------------------------------------
    void DamageComponent::SetBaseDamage(float amount)
    {
        m_baseDamage = amount;
    }
    //--------------------------------------------------------------------------------------------------------
    void DamageComponent::SetHitRate(float rate)
    {
        m_hitRate = rate;
    }
    //--------------------------------------------------------------------------------------------------------
    void DamageComponent::SetDamageType(int typeID)
    {
        m_damageTypeID = typeID;
    }
    //--------------------------------------------------------------------------------------------------------
    float DamageComponent::GetBaseDamage() const
    {
        return m_baseDamage;
    }
    //--------------------------------------------------------------------------------------------------------
    float DamageComponent::GetHitRate() const
    {
        return m_hitRate;
    }
    //--------------------------------------------------------------------------------------------------------
    int DamageComponent::GetDamageTypeID() const
    {
        return m_damageTypeID;
    }
    //--------------------------------------------------------------------------------------------------------
    bool DamageComponent::GetReadyToHit() const
    {
        return m_readyToHit;
    }
    //--------------------------------------------------------------------------------------------------------
    void DamageComponent::Update(float deltaTime)
    {
        if (!m_readyToHit && m_hitCooldown < m_hitRate)
        {
            if (m_hitCooldown < m_hitRate)
            {
                m_hitCooldown += deltaTime;
            }
            m_readyToHit = true;
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void DamageComponent::Render(){}
}