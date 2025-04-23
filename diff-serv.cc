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

Ptr<Packet>
DiffServ::Remove()
{
    return Schedule();
}

Ptr<const Packet>
DiffServ::Peek() const
{
}

} // namespace ns3

int
main()
{
    return 0;
}