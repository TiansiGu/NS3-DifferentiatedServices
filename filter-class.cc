#include "filter-class.h"

namespace ns3
{
TypeId
Filter::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Filter").SetParent<Object>().AddConstructor<Filter>();
    return tid;
}

Filter::Filter()
{
}

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

void
Filter::AddFilterElement(FilterElement* filterElement)
{
    elements.push_back(filterElement);
}

} // namespace ns3