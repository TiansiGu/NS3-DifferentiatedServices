#include "drr-queue.h"

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

DrrQueue::DrrQueue()
    : m_currentIndex(0)
{
}

TypeId
DrrQueue::GetTypeId()
{
    static TypeId tid = TypeId("ns3::DrrQueue")
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

void
DrrQueue::DoInitialize()
{
    if (!m_configFile.empty())
    {
        LoadQuantumConfigFromFile(m_configFile);
    }
    m_deficitCounters.resize(m_quantums.size(), 0);
    // DiffServ::DoInitialize();
}

void
DrrQueue::LoadQuantumConfigFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        // NS_LOG_ERROR("Cannot open DRR config file: " << filename);
        NS_LOG_COMPONENT_DEFINE("DrrQueue");
        // NS_LOG_COMPONENT_ERROR("DrrQueue", "Cannot open DRR config file: " << filename);

        std::ostringstream oss;
        oss << "Cannot open DRR config file: " << filename;
        NS_LOG_COMPONENT_ERROR("DrrQueue", oss.str());

        // NS_LOG_LOGIC("Cannot open DRR config file: " << filename);

        return;
    }

    uint32_t count;
    file >> count;
    m_quantums.clear();

    for (uint32_t i = 0; i < count; ++i)
    {
        uint32_t q;
        file >> q;
        m_quantums.push_back(q);
    }

    file.close();
    // NS_LOG_INFO("Loaded " << m_quantums.size() << " quantum values.");
    // MS_LOG_LOGIC();
}

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

Ptr<Packet>
DrrQueue::Schedule()
{
    const std::vector<Ptr<TrafficClass>>& classes = GetTrafficClasses();
    uint32_t n = classes.size();

    while (true)
    {
        bool anyQueueHasPacket = false;

        for (uint32_t offset = 0; offset < n; ++offset)
        {
            uint32_t i = m_currentIndex;
            Ptr<TrafficClass> tc = classes[i];

            // uint32_t i = (m_currentIndex + offset) % n;

            // Skip empty queues
            if (tc->GetPackets() == 0)
            {
                // m_deficitCounters[i] += m_quantums[i];
                m_currentIndex = (m_currentIndex + 1) % n;
                continue;
            }

            // Add quantum to current deficit only once per round
            // m_deficitCounters[i] += m_quantums[i];

            if (classes[i]->GetPackets() > 0)
            {
                // Only when visited, increase its deficit
                // m_deficitCounters[i] += m_quantums[i];

                anyQueueHasPacket = true;

                Ptr<Packet> p = classes[i]->Peek();
                if (p && p->GetSize() <= m_deficitCounters[i])
                {
                    m_deficitCounters[i] -= p->GetSize();
                    // m_currentIndex = (i + 1) % n;
                    Ptr<Packet> sendPacket = classes[i]->Dequeue();

                    // only move m_currentIndex if 1. the queue is empty 2. next Packet is larger
                    // than deficit
                    if (tc->GetPackets() != 0 && tc->Peek()->GetSize() > m_deficitCounters[i])
                    {
                        m_deficitCounters[i] += m_quantums[i];
                        m_currentIndex = (i + 1) % n;
                    }
                    // return classes[i]->Dequeue();
                    return sendPacket;
                }
                else
                {
                    m_deficitCounters[i] += m_quantums[i];
                    m_currentIndex = (i + 1) % n;
                }
            }

            // // only move m_currentIndex if 1. the queue is empty 2. next Packet is larger than
            // deficit if (tc->GetPackets() == 0 || tc->Peek()->GetSize() > m_deficitCounters[i])
            // {
            //     m_deficitCounters[i] += m_quantums[i];
            //     m_currentIndex = (i + 1) % n;
            // }
        }

        if (!anyQueueHasPacket)
        {
            return nullptr; // no packets
        }

        // if not enough deficit, try next round
    }
}

} // namespace ns3
