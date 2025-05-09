/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai <kdai3@dons.usfca.edu>, Tiansi Gu <tgu10@dons.usfca.edu>
 */

#include "traffic-class.h"

namespace ns3
{
NS_OBJECT_ENSURE_REGISTERED(TrafficClass);

TypeId
TrafficClass::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::TrafficClass")
            .SetParent<Object>()
            .AddConstructor<TrafficClass>()

            // Register maxPackets
            .AddAttribute("maxPackets",
                          "Maximum number of packets in the class queue",
                          UintegerValue(100),
                          MakeUintegerAccessor(&TrafficClass::maxPackets),
                          MakeUintegerChecker<uint32_t>())

            // Register isDefault
            .AddAttribute("isDefault",
                          "Whether this is the default traffic class",
                          BooleanValue(false),
                          MakeBooleanAccessor(&TrafficClass::isDefault),
                          MakeBooleanChecker())

            // Register priority_level
            .AddAttribute("priority_level",
                          "Priority level (higher number indicates higher priority)",
                          UintegerValue(0),
                          MakeUintegerAccessor(&TrafficClass::priority_level),
                          MakeUintegerChecker<uint32_t>())

            // Register weight
            .AddAttribute("weight",
                          "quantum of the traffic clas",
                          UintegerValue(1000),
                          MakeUintegerAccessor(&TrafficClass::weight),
                          MakeUintegerChecker<uint32_t>());

    return tid;
}

TrafficClass::TrafficClass()
    : packets(0)
{
}

/**
 * @brief Returns true if this traffic class is the default fallback class
 */
bool
TrafficClass::IsDefault() const
{
    return isDefault;
}

/**
 * @brief Attempts to enqueue a packet into the traffic class
 *
 * @param p Packet to enqueue
 * @return true if successful, false if the queue is full
 */
bool
TrafficClass::Enqueue(Ptr<ns3::Packet> p)
{
    if (packets == maxPackets)
        return false;

    m_queue.push(p);
    packets++;

    return true;
}

/**
 * @brief Dequeues and returns the next packet in the queue
 *
 * @return Ptr to the dequeued packet, or nullptr if queue is empty
 */
Ptr<Packet>
TrafficClass::Dequeue()
{
    if (packets == 0)
        return nullptr;

    Ptr<Packet> p = m_queue.front();

    m_queue.pop();
    packets--;
    return p;
}

/**
 * @brief Check if the packet matches any of the configured filters
 *
 * @param p Packet to check
 * @return true if any filter matches, false otherwise
 */
bool
TrafficClass::Match(Ptr<Packet> p) const
{
    for (Ptr<Filter> filter : filters)
    {
        if (filter->Match(p))
            return true;
    }
    return false;
}

/**
 * @brief Returns the number of packets currently in the queue
 */
uint32_t
TrafficClass::GetPackets() const
{
    return packets;
}

/**
 * @brief Returns the front packet in the queue without removing it
 *
 * @return Ptr to the packet, or nullptr if queue is empty
 */
Ptr<Packet>
TrafficClass::Peek() const
{
    if (packets == 0)
        return nullptr;

    return m_queue.front();
}

/**
 * @brief Returns the priority level assigned to this traffic class
 */
uint32_t
TrafficClass::GetPriorityLevel() const
{
    return priority_level;
}

/**
 * @brief Returns the quantum/weight assigned to this traffic class
 */
uint32_t
TrafficClass::GetWeight() const
{
    return weight;
}

/**
 * @brief Adds a new filter to this traffic class
 *
 * @param filter Ptr to a filter instance
 */
void
TrafficClass::AddFilter(Ptr<Filter> filter)
{
    filters.push_back(filter);
}

} // namespace ns3
