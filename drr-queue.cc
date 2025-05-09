#include "drr-queue.h"

#include "qos-initializer.h"

#include "ns3/log.h"
#include "ns3/string.h"

#include <fstream>
#include <sstream>

#define NS_LOG_COMPONENT_INFO(comp, msg) std::cout << "[" << comp << " INFO] " << msg << std::endl;

#define NS_LOG_COMPONENT_ERROR(comp, msg)                                                          \
    std::cerr << "[" << comp << " ERROR] " << msg << std::endl;

#define NS_LOG_COMPONENT_DEBUG(comp, msg)                                                          \
    std::cout << "[" << comp << " DEBUG] " << msg << std::endl;

NS_LOG_COMPONENT_DEFINE("DrrQueue");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(DrrQueue);

// Constructor initializes current index to 0
DrrQueue::DrrQueue()
    : m_currentIndex(0)
{
}

// TypeId registration with ns-3
TypeId
DrrQueue::GetTypeId()
{
    static TypeId tid = TypeId("ns3::DrrQueue<Packet>")
                            .SetParent<DiffServ>()
                            .SetGroupName("Network")
                            .AddConstructor<DrrQueue>()
                            .AddAttribute("Config",
                                          "Path to DRR configuration file",
                                          StringValue(""),
                                          MakeStringAccessor(&DrrQueue::m_configFile),
                                          MakeStringChecker());
    return tid;
}

/**
 * @brief Initializes the DRR queue using a JSON config file.
 *        Parses the configuration, sets up TrafficClasses,
 *        and initializes deficit counters.
 */
void
DrrQueue::DoInitialize()
{
    DiffServ::DoInitialize();
    QoSInitializer::InitializeDrrFromJson(this, m_configFile);
    m_deficitCounters.resize(GetTrafficClasses().size(), 0);
}

/**
 * @brief Classify incoming packets based on filters in TrafficClass.
 *        Returns the index of the first matching class, or the default class.
 */
uint32_t
DrrQueue::Classify(Ptr<Packet> p)
{
    const std::vector<Ptr<TrafficClass>>& classes = GetTrafficClasses();
    for (uint32_t i = 0; i < classes.size(); ++i)
    {
        if (classes[i]->Match(p))
            return i;
    }

    // fallback: return the default queue if available
    for (uint32_t i = 0; i < classes.size(); ++i)
    {
        if (classes[i]->IsDefault())
            return i;
    }

    return 0;
}


/**
 * @brief Schedules the next packet for transmission using the DRR algorithm.
 *        Internally calls GetQueueForSchedule to find the eligible class.
 * @return A pointer to the packet to be dequeued, or nullptr if all queues are empty.
 */
Ptr<Packet>
DrrQueue::Schedule()
{
    int scheduleIndex = GetQueueForSchedule();

    if (scheduleIndex == -1)
    {
        return nullptr;
    }

    const std::vector<Ptr<TrafficClass>>& classes = GetTrafficClasses();
    Ptr<TrafficClass> tc = classes[scheduleIndex];
    Ptr<Packet> p = tc->Dequeue();
    m_deficitCounters[scheduleIndex] -= p->GetSize();

    return p;
}

/**
 * @brief Determines which traffic class should be scheduled next, based on the DRR policy.
 *        Iterates over the queue in a round-robin manner and updates deficit counters.
 * @return Index of the selected traffic class, or -1 if all queues are empty.
 */
uint32_t
DrrQueue::GetQueueForSchedule() const
{
    const std::vector<Ptr<TrafficClass>>& classes = GetTrafficClasses();
    uint32_t n = classes.size();

    while (true)
    {
        bool anyQueueHasPacket = false;
        for (uint32_t offset = 0; offset < n; ++offset)
        {
            Ptr<TrafficClass> tc = classes[m_currentIndex];

            // Skip empty queues as they are not in the activate list
            if (tc->GetPackets() == 0)
            {
                m_deficitCounters[m_currentIndex] = 0;
                m_currentIndex = (m_currentIndex + 1) % n;
                continue;
            }

            // At least one queue is not empty
            anyQueueHasPacket = true;
            Ptr<Packet> p = classes[m_currentIndex]->Peek();
            // If packet size is smaller then available deficit, it could be schedule next
            if (p && p->GetSize() <= m_deficitCounters[m_currentIndex])
            {
                return m_currentIndex;
            }
            // When the quantum is not enough, add quantum and move to the next queue. We add
            // quantum before leaving, which is the same as adding quantum the next round this
            // queue is reached, and guarantees quantum is added only once into any queue for each
            // round. This solution also works when the deficit counters are all zeros at the very
            // start: in the first round, we only add quantum and will not schedule any packet,
            // but expected DRR scheduling will start from the second round.
            else
            {
                m_deficitCounters[m_currentIndex] += tc->GetWeight();
                m_currentIndex = (m_currentIndex + 1) % n;
            }
        }

        // We will stop the loop if all of the queues in q_class are empty
        if (!anyQueueHasPacket)
        {
            return -1; // no packets
        }
    }
}

} // namespace ns3
