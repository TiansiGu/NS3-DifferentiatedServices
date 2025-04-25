#include "spq.h"

// #include "creators/qos-initializer.h"
#include "qos-initializer.h"

#include "ns3/string.h"

namespace ns3
{
StrictPriorityQueue::StrictPriorityQueue()
{
    // Empty initialization: c++ automatically initializes empty vector
}

TypeId
StrictPriorityQueue::GetTypeId()
{
    static TypeId tid = TypeId("ns3::StrictPriorityQueue")
                            .SetParent<DiffServ>()
                            .SetGroupName("Network")
                            .AddConstructor<StrictPriorityQueue>()
                            .AddAttribute("Config",
                                          "Path to DRR configuration file",
                                          StringValue(""),
                                          MakeStringAccessor(&StrictPriorityQueue::m_configFile),
                                          MakeStringChecker());
    ;
    return tid;
}

u_int32_t
StrictPriorityQueue::Classify(Ptr<Packet> p)
{
    const auto& q_class = GetTrafficClasses();
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
    const auto& q_class = GetTrafficClasses();
    for (TrafficClass* queue_class : q_class)
    {
        if (queue_class->GetPackets() > 0)
        {
            return queue_class->Dequeue();
        }
    }

    return nullptr;
}

void
StrictPriorityQueue::AddTrafficClass(TrafficClass* trafficClass)
{
    DiffServ::AddTrafficClass(trafficClass);

    auto& q_class = GetTrafficClasses();
    std::sort(q_class.begin(), q_class.end(), [](const TrafficClass* a, const TrafficClass* b) {
        return a->GetPriorityLevel() > b->GetPriorityLevel(); // descending order
    });
}

void
StrictPriorityQueue::DoInitialize()
{
    DiffServ::DoInitialize();
    QoSInitializer::InitializeSpqFromJson(this, m_configFile);
}

} // namespace ns3