#include "drr-queue.h"
#include "ns3/string.h"
#include "ns3/log.h"

#include <fstream>
#include <sstream>

// NS_LOG_COMPONENT_DEFINE("DrrQueue");

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
    const std::vector<TrafficClass*>& classes = GetTrafficClasses();
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
    const std::vector<TrafficClass*>& classes = GetTrafficClasses();
    uint32_t n = classes.size();

    while (true)
    {
        bool anyQueueHasPacket = false;

        for (uint32_t offset = 0; offset < n; ++offset)
        {
            uint32_t i = (m_currentIndex + offset) % n;

            if (classes[i]->GetPackets() > 0)
            {
                // Only when visited, increase its deficit
                m_deficitCounters[i] += m_quantums[i];

                anyQueueHasPacket = true;

                Ptr<Packet> p = classes[i]->Peek();
                if (p && p->GetSize() <= m_deficitCounters[i])
                {
                    m_deficitCounters[i] -= p->GetSize();
                    m_currentIndex = (i + 1) % n;
                    return classes[i]->Dequeue();
                }
            }
        }

        if (!anyQueueHasPacket)
        {
            return nullptr; // no packets
        }

        // if not enough deficit, try next round
    }
}

} // namespace ns3
