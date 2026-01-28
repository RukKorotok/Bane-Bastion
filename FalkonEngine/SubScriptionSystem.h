#pragma once
#include "GameEvents.h"

namespace FalkonEngine
{
    class Observable;
    class Observer;

    namespace Details 
    {
        struct SubscriptionNode 
        {
            class Observable* subject;
            class Observer* observer;

            SubscriptionNode* nextInSub = nullptr;
            SubscriptionNode* prevInSub = nullptr;
            SubscriptionNode* nextInObs = nullptr;
            SubscriptionNode* prevInObs = nullptr;
        };
    }

    class Observer 
    {
    public:
        virtual ~Observer();
        virtual void OnNotify(const GameEvent& event) = 0;

    private:
        friend class Observable;

        void _Internal_Detach(Details::SubscriptionNode* n);
        Details::SubscriptionNode* m_head = nullptr;

    };

    class Observable 
    {
    public:
        virtual ~Observable();
        void Subscribe(Observer* obs);
        void Notify(const GameEvent& event);

    private:
        friend class Observer;

        void _Internal_Detach(Details::SubscriptionNode* node);
        Details::SubscriptionNode* m_head = nullptr;
    };
}