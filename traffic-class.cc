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

            // Register priorityLevel
            .AddAttribute("priorityLevel",
                          "Priority level (higher number indicates higher priority)",
                          UintegerValue(0),
                          MakeUintegerAccessor(&TrafficClass::priorityLevel),
                          MakeUintegerChecker<uint32_t>());

    return tid;
}

TrafficClass::TrafficClass()
{
}

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
    // NS_LOG_UNCOND("filters matching");
    for (Ptr<Filter> filter : filters)
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

Ptr<Packet>
TrafficClass::Peek() const
{
    if (packets == 0)
        return nullptr;

    return m_queue.front();
}

uint32_t
TrafficClass::GetPriorityLevel() const
{
    return priorityLevel;
}

void
TrafficClass::AddFilter(Ptr<Filter> filter)
{
    filters.push_back(filter);
}

} // namespace ns3