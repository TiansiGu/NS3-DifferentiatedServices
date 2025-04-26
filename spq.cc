#include "spq.h"

// #include "creators/qos-initializer.h"
#include "qos-initializer.h"

#include "ns3/string.h"

namespace ns3
{
NS_OBJECT_ENSURE_REGISTERED(StrictPriorityQueue);

StrictPriorityQueue::StrictPriorityQueue()
{
    // Empty initialization: c++ automatically initializes empty vector
    NS_LOG_UNCOND("Create empty SPQ");
}

TypeId
StrictPriorityQueue::GetTypeId()
{
    static TypeId tid = TypeId("ns3::StrictPriorityQueue<Packet>")
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
        Ptr<TrafficClass> queue_class = q_class[i];
        if (queue_class->Match(p))
        {
            return i;
        }
    }

    // If a packet doesn't match any of the queue, place it in the default queue
    for (int i = 0; i < q_class.size(); i++)
    {
        Ptr<TrafficClass> queue_class = q_class[i];
        if (queue_class->IsDefault())
        {
            return i;
        }
    }
}

// Ptr<Packet>
// StrictPriorityQueue::Schedule()
// {
//     NS_LOG_UNCOND("start StrictPriorityQueue  Schedule");
//     const auto& q_class = GetTrafficClasses();
//     for (Ptr<TrafficClass> queue_class : q_class)
//     {
//         if (queue_class->GetPackets() > 0)
//         {
//             return queue_class->Dequeue();
//         }
//     }

//     return nullptr;
// }

Ptr<Packet>
StrictPriorityQueue::Schedule()
{
    NS_LOG_UNCOND("start StrictPriorityQueue Schedule");
    const auto& q_class = GetTrafficClasses();

    for (uint32_t i = 0; i < q_class.size(); ++i)
    {
        Ptr<TrafficClass> queue_class = q_class[i];
        NS_LOG_UNCOND("Checking queue index " << i
                        << " priority " << queue_class->GetPriorityLevel()
                        << " packets " << queue_class->GetPackets());

        if (queue_class->GetPackets() > 0)
        {
            NS_LOG_UNCOND("Dequeue from queue index " << i
                            << " priority " << queue_class->GetPriorityLevel()
                            << " packets before dequeue: " << queue_class->GetPackets());

            Ptr<Packet> pkt = queue_class->Dequeue();

            if (pkt != nullptr)
            {
                NS_LOG_UNCOND("Dequeued packet UID: " << pkt->GetUid()
                                << " Size: " << pkt->GetSize()
                                << " from queue index " << i);
            }
            else
            {
                NS_LOG_UNCOND("Dequeued packet is nullptr!!");
            }

            return pkt;
        }
    }

    NS_LOG_UNCOND("No non-empty queue found, returning nullptr");
    return nullptr;
}

void
StrictPriorityQueue::AddTrafficClass(Ptr<TrafficClass> trafficClass)
{
    DiffServ::AddTrafficClass(trafficClass);

    auto& q_class = GetTrafficClasses();
    std::sort(q_class.begin(),
              q_class.end(),
              [](const Ptr<TrafficClass> a, const Ptr<TrafficClass> b) {
                  return a->GetPriorityLevel() > b->GetPriorityLevel(); // descending order
              });
}

void
StrictPriorityQueue::DoInitialize()
{
    NS_LOG_UNCOND("SPQ DoInitialize start");
    DiffServ::DoInitialize();
    QoSInitializer::InitializeSpqFromJson(this, m_configFile);
}

} // namespace ns3