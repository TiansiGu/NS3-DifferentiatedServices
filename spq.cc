#include "spq.h"

namespace ns3
{
u_int32_t
StrictPriorityQueue::Classify(Ptr<Packet> p)
{
    const std::vector<TrafficClass*> q_class = GetTrafficClasses();
    for (int i = 0; i < q_class.size(); i++)
    {
        TrafficClass* queue_class = q_class[i];
        if (queue_class->Match(p))
        {
            return i;
        }
    }

    // If a packet doesn't match any of the queue, place it in the default queue
    for (int i = 0; i < q_class.size(); i++)
    {
        TrafficClass* queue_class = q_class[i];
        if (queue_class->IsDefault())
        {
            return i;
        }
    }
}

Ptr<Packet>
StrictPriorityQueue::Schedule()
{
    for (TrafficClass* queue_class : GetTrafficClasses())
    {
        if (queue_class->GetPackets() > 0)
        {
            return queue_class->Dequeue();
        }
    }

    return nullptr;
}

} // namespace ns3