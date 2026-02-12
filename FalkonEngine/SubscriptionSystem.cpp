#include "pch.h"
#include "SubscriptionSystem.h"

namespace FalkonEngine
{
    //Observer
    //--------------------------------------------------------------------------------------------------------
    Observer::~Observer() 
    {
        FE_CORE_INFO("Observer is being destroyed. Cleaning up subscriptions.");

        while (m_head) 
        {
            Details::SubscriptionNode* next = m_head->nextInObs;

            FE_CORE_ASSERT(m_head->subject != nullptr, "SubscriptionNode has null subject during Observer destruction!");

            m_head->subject->_Internal_Detach(m_head);
            delete m_head;

            m_head = next;
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void Observer::_Internal_Detach(Details::SubscriptionNode* node) 
    {
        FE_CORE_ASSERT(node != nullptr, "Attempted to detach a null node in Observer.");

        if (node->prevInObs) 
        {
            node->prevInObs->nextInObs = node->nextInObs;
        }
        if (node->nextInObs) node->nextInObs->prevInObs = node->prevInObs;
        if (m_head == node) m_head = node->nextInObs;
    }
    //Observable
    //--------------------------------------------------------------------------------------------------------
    Observable::~Observable() 
    {
        FE_CORE_INFO("Observable is being destroyed. Detaching all observers.");

        while (m_head) 
        {
            Details::SubscriptionNode* next = m_head->nextInSub;
 
            FE_CORE_ASSERT(m_head->subject != nullptr, "SubscriptionNode has null subject during Observer destruction!");

            if (m_head->observer)
            {
                m_head->observer->_Internal_Detach(m_head);
            }
            else
            {
                FE_CORE_WARN("SubscriptionNode has null observer during Observable destruction.");
            }

            delete m_head;
            m_head = next;
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void Observable::Subscribe(Observer* obs) 
    {
        if (!obs) 
        {
            FE_CORE_ERROR("Attempted to subscribe a null Observer to an Observable!");
            return;
        }

        auto* node = new Details::SubscriptionNode{ this, obs };

        node->nextInSub = m_head;
        if (m_head) m_head->prevInSub = node;
        m_head = node;

        node->nextInObs = obs->m_head;
        if (obs->m_head) obs->m_head->prevInObs = node;
        obs->m_head = node;

        FE_APP_TRACE("New subscription established between Observable and Observer.");
    }
    //--------------------------------------------------------------------------------------------------------
    void Observable::Notify(const GameEvent& event) 
    {
        Details::SubscriptionNode* curr = m_head;
        int notifyCount = 0;

        while (curr) 
        {
            Details::SubscriptionNode* next = curr->nextInSub;

            if (curr->observer) 
            {
                curr->observer->OnNotify(event);
                notifyCount++;
            }
            else
            {
                FE_CORE_ERROR("Broken SubscriptionNode detected during Notify: Observer is null!");
            }
            curr = next;
        }
        //For deep debug
        // FE_APP_TRACE("Event notified to " + std::to_string(notifyCount) + " observers.");
    }
    //--------------------------------------------------------------------------------------------------------
    void Observable::_Internal_Detach(Details::SubscriptionNode* node)
    {
        FE_CORE_ASSERT(node != nullptr, "Attempted to detach a null node in Observable.");

        if (node->prevInSub) 
        {
            node->prevInSub->nextInSub = node->nextInSub;
        }
        if (node->nextInSub) 
        {
            node->nextInSub->prevInSub = node->prevInSub;
        }
        if (m_head == node) 
        {
            m_head = node->nextInSub;
        }
    }
}