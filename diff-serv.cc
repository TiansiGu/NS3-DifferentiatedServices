/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai <kdai3@dons.usfca.edu>, Tiansi Gu <tgu10@dons.usfca.edu>
 */

#include "diff-serv.h"

namespace ns3
{

/**
 * @brief Enqueue a packet into the appropriate TrafficClass queue.
 *
 * @param p The packet to enqueue.
 * @return true if enqueue succeeds; false if the packet doesn't match any queue, or the
 * appropriate TrafficClass queue is full.
 */
bool
DiffServ::Enqueue(Ptr<Packet> p)
{
    int32_t index = Classify(p);

    if (index < 0)
    {
        return false;
    }

    Ptr<TrafficClass> queue_class = q_class.at(index);

    return queue_class->Enqueue(p);
}

/**
 * @brief Dequeue a packet based on the scheduling algorithm.
 *
 * @return The dequeued packet, or nullptr if all queues are empty.
 */
Ptr<Packet>
DiffServ::Dequeue()
{
    return Schedule();
}

/**
 * @brief Peek at the first packet from the first non-empty TrafficClass without removing it.
 *        Scans the traffic classes in order and returns the first non-empty one.
 *
 * @return The front packet of the first non-empty queue, or nullptr if empty.
 */
Ptr<const Packet>
DiffServ::Peek() const
{
    int index = GetQueueForSchedule();

    if (index < 0 || q_class[index]->GetPackets() == 0)
    {
        return nullptr;
    }

    Ptr<TrafficClass> queue = q_class[index];
    return queue->Peek();
}

/**
 * @brief Remove a packet from the queue (alias for Dequeue).
 *
 * @return The removed packet, or nullptr if all queues are empty.
 */
Ptr<Packet>
DiffServ::Remove()
{
    return Schedule();
}

/**
 * @brief Access the modifiable list of traffic classes.
 *
 * @return Reference to the vector of traffic classes.
 */
std::vector<Ptr<TrafficClass>>&
DiffServ::GetTrafficClasses()
{
    return q_class;
}

/**
 * @brief Access the const list of traffic classes.
 *
 * @return Const reference to the vector of traffic classes.
 */
const std::vector<Ptr<TrafficClass>>&
DiffServ::GetTrafficClasses() const
{
    return q_class;
}

/**
 * @brief Add a new traffic class to the internal list.
 *
 * @param trafficClass The TrafficClass object to add.
 */
void
DiffServ::AddTrafficClass(Ptr<TrafficClass> trafficClass)
{
    q_class.push_back(trafficClass);
}

} // namespace ns3
