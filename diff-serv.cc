#include "diff-serv.h"

namespace ns3
{

bool
DiffServ::Enqueue(Ptr<Packet> p)
{
    uint32_t index = Classify(p);
    TrafficClass* queue_class = q_class.at(index);
    return queue_class->Enqueue(p);
}

Ptr<Packet>
DiffServ::Dequeue()
{
    return Schedule();
}

Ptr<const Packet>
DiffServ::Peek() const
{
    return nullptr;
}

Ptr<Packet>
DiffServ::Remove()
{
    return Schedule();
}

std::vector<TrafficClass*>&
DiffServ::GetTrafficClasses()
{
    return q_class;
}

const std::vector<TrafficClass*>&
DiffServ::GetTrafficClasses() const
{
    return q_class;
}

void
DiffServ::AddTrafficClass(TrafficClass* trafficClass)
{
    q_class.push_back(trafficClass);
}

} // namespace ns3

int
main()
{
    return 0;
}