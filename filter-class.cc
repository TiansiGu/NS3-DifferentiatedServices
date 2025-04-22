#include "filter.h"

namespace ns3
{

bool
Filter::match(Ptr<Packet> p) const
{
    for (FilterElement* element : elements)
    {
        if (!element->match(p))
            return false;
    }
    return true;
}

} // namespace ns3