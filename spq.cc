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
    // NS_LOG_UNCOND("Create empty SPQ");
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

/**
 * @brief Initialization hook called at simulation startup.
 *
 * Parses the JSON configuration file and creates the configured traffic classes.
 */
void
StrictPriorityQueue::DoInitialize()
{
    // NS_LOG_UNCOND("SPQ DoInitialize start");
    DiffServ::DoInitialize();
    QoSInitializer::InitializeSpqFromJson(this, m_configFile);
}

/**
 * @brief Classify an incoming packet into a traffic class index based on filter rules.
 *
 * First matches filters; if no match is found, returns the index of the default queue.
 *
 * @param p The incoming packet
 * @return The index of the matching or default TrafficClass
 */
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

/**
 * @brief Schedules the next packet for transmission based on strict priority logic.
 *
 * Always selects the first available packet from the highest-priority non-empty queue.
 *
 * @return Pointer to the selected packet, or nullptr if no packet is available.
 */
Ptr<Packet>
StrictPriorityQueue::Schedule()
{
    int scheduleIndex = GetQueueForSchedule();
    if (scheduleIndex == -1)
    {
        NS_LOG_UNCOND("No non-empty queue found, returning nullptr");
        return nullptr;
    }

    const std::vector<Ptr<TrafficClass>>& classes = GetTrafficClasses();
    Ptr<TrafficClass> tc = classes[scheduleIndex];
    return tc->Dequeue();
}

/**
 * @brief Adds a new traffic class and sorts all classes by descending priority.
 *
 * Ensures the highest-priority class is always first for scheduling purposes.
 *
 * @param trafficClass Pointer to the new TrafficClass
 */
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

/**
 * @brief Finds the index of the highest-priority non-empty traffic class.
 *
 * Used by the scheduler to determine which queue should be dequeued next.
 *
 * @return Index of the selected traffic class, or -1 if all queues are empty.
 */
uint32_t
StrictPriorityQueue::GetQueueForSchedule() const
{
    const auto& q_class = GetTrafficClasses();

    for (uint32_t i = 0; i < q_class.size(); ++i)
    {
        Ptr<TrafficClass> queue_class = q_class[i];

        if (queue_class->GetPackets() > 0)
        {
            return i;
        }
    }
    return -1;
}

} // namespace ns3