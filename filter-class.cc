#include "filter.h"

namespace ns3
{

bool
Filter::Match(Ptr<Packet> p) const
{
    for (FilterElement* element : elements)
    {
        if (!element->Match(p))
            return false;
    }
    return true;
}

} // namespace ns3