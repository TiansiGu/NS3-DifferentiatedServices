#include "diff-serv.h"

namespace ns3
{

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

Ptr<Packet>
DiffServ::Dequeue()
{
    // NS_LOG_UNCOND("start dequeue");
    return Schedule();
}

// peek the first packet from the first non-empty TrafficClass
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

Ptr<Packet>
DiffServ::Remove()
{
    return Schedule();
}

std::vector<Ptr<TrafficClass>>&
DiffServ::GetTrafficClasses()
{
    return q_class;
}

const std::vector<Ptr<TrafficClass>>&
DiffServ::GetTrafficClasses() const
{
    return q_class;
}

void
DiffServ::AddTrafficClass(Ptr<TrafficClass> trafficClass)
{
    q_class.push_back(trafficClass);
}

} // namespace ns3
