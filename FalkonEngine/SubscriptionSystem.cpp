#include "pch.h"
#include "SubscriptionSystem.h"

namespace FalkonEngine
{
    //Observer
    //--------------------------------------------------------------------------------------------------------
    Observer::~Observer() 
    {
        while (m_head) 
        {
            Details::SubscriptionNode* next = m_head->nextInObs;

            m_head->subject->_Internal_Detach(m_head);
            delete m_head;

            m_head = next;
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void Observer::_Internal_Detach(Details::SubscriptionNode* node) 
    {
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
        while (m_head) 
        {
            Details::SubscriptionNode* next = m_head->nextInSub;
 
            m_head->observer->_Internal_Detach(m_head);

            delete m_head;
            m_head = next;
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void Observable::Subscribe(Observer* obs) 
    {
        if (!obs) 
        {
            return;
        }

        auto* node = new Details::SubscriptionNode{ this, obs };

        node->nextInSub = m_head;
        if (m_head) m_head->prevInSub = node;
        m_head = node;

        node->nextInObs = obs->m_head;
        if (obs->m_head) obs->m_head->prevInObs = node;
        obs->m_head = node;
    }
    //--------------------------------------------------------------------------------------------------------
    void Observable::Notify(const GameEvent& event) 
    {
        Details::SubscriptionNode* curr = m_head;
        while (curr) 
        {
            Details::SubscriptionNode* next = curr->nextInSub;

            if (curr->observer) 
            {
                curr->observer->OnNotify(event);
            }
            curr = next;
        }
    }
    //--------------------------------------------------------------------------------------------------------
    void Observable::_Internal_Detach(Details::SubscriptionNode* node)
    {
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