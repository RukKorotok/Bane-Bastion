#pragma once
#include "Component.h"

namespace FalkonEngine
{
    class DamageComponent : public Component
    {
    public:
        DamageComponent(GameObject* gameObject);
        virtual ~DamageComponent() = default;

        float HitTrigger();

        void SetBaseDamage(float amount);
        void SetHitRate(float rate);
        void SetDamageType(int typeID);

        float GetBaseDamage() const;
        float GetHitRate() const;
        int GetDamageTypeID() const;
        bool GetReadyToHit() const;

        void Update(float deltaTime) override;
        void Render() override;

    private:
        float m_baseDamage = 0.0f;
        float m_hitRate = 1.0f;
        float m_hitCooldown = 0.0f;
        bool m_readyToHit = true;
        int m_damageTypeID = 0;

    };
}