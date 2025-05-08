#include "diff-serv.h"

namespace ns3
{

/**
 * @brief Enqueue a packet into the appropriate TrafficClass queue.
 *
 * @param p The packet to enqueue.
 * @return true if enqueue succeeds; false if the sub-queue is full.
 */
bool
DiffServ::Enqueue(Ptr<Packet> p)
{
    // NS_LOG_UNCOND("Enqueue packet UID: " << p->GetUid() << " Size: " << p->GetSize() << " Ptr: "
    // << p);
    uint32_t index = Classify(p);
    // NS_LOG_UNCOND("enqueue to index" << index);
    Ptr<TrafficClass> queue_class = q_class.at(index);
    // NS_LOG_UNCOND("queue class size " << q_class.size());
    // NS_LOG_UNCOND("Enqueue packet UID22222: " << p->GetUid() << " Size: " << p->GetSize() << "
    // Ptr: " << p);

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
    // NS_LOG_UNCOND("start dequeue");
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
    for (const auto& tc : q_class)
    {
        if (tc->GetPackets() > 0)
        {
            return tc->Peek();
        }
    }
    return nullptr; // all queue empty
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
