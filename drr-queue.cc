#include "drr-queue.h"
#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("DrrQueue");

DrrQueue::DrrQueue(std::vector<uint32_t> quantumValues)
    : m_quantums(quantumValues),
      m_currentIndex(0)
{
    m_deficitCounters.resize(quantumValues.size(), 0);
}

uint32_t
DrrQueue::Classify(Ptr<Packet> p)
{
    for (uint32_t i = 0; i < q_class.size(); ++i)
    {
        if (q_class[i]->Match(p))
            return i;
    }

    // fallback: return the default queue if available
    for (uint32_t i = 0; i < q_class.size(); ++i)
    {
        if (q_class[i]->IsDefault())
            return i;
    }

    return 0;
}

Ptr<Packet>
DrrQueue::Schedule()
{
    uint32_t n = q_class.size();

    while (true)
    {
        bool anyQueueHasPacket = false;

        for (uint32_t offset = 0; offset < n; ++offset)
        {
            uint32_t i = (m_currentIndex + offset) % n;

            if (!q_class[i]->Empty())
            {
                // Only when visited, increase its deficit
                m_deficitCounters[i] += m_quantums[i];

                anyQueueHasPacket = true;

                Ptr<Packet> p = q_class[i]->Peek();
                if (p && p->GetSize() <= m_deficitCounters[i])
                {
                    m_deficitCounters[i] -= p->GetSize();
                    m_currentIndex = (i + 1) % n; 
                    return q_class[i]->Dequeue();
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

Ptr<const Packet>
DrrQueue::Peek() const
{
    uint32_t n = q_class.size();
    uint32_t idx = m_currentIndex;

    for (uint32_t offset = 0; offset < n; ++offset)
    {
        uint32_t i = (idx + offset) % n;

        if (!q_class[i]->Empty())
        {
            Ptr<Packet> p = q_class[i]->Peek();
            if (p && p->GetSize() <= m_deficitCounters[i] + m_quantums[i])
            {
                return p;
            }
        }
    }

    return nullptr;
}

} // namespace ns3
