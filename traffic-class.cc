#include "traffic-class.h"

namespace ns3
{
bool
TrafficClass::IsDefault() const
{
    return isDefault;
}

bool
TrafficClass::Enqueue(Ptr<ns3::Packet> p)
{
    if (packets == maxPackets)
        return false;

    m_queue.push(p);
    packets++;
    return true;
}

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

bool
TrafficClass::Match(Ptr<Packet> p) const
{
    for (Filter* filter : filters)
    {
        if (filter->Match(p))
            return true;
    }
    return false;
}

uint32_t
TrafficClass::GetPackets() const
{
    return packets;
}

} // namespace ns3