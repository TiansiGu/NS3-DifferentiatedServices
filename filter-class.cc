#include "filter-class.h"

namespace ns3
{
NS_OBJECT_ENSURE_REGISTERED(Filter);

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
    // NS_LOG_UNCOND("filters matching");
    for (Ptr<FilterElement> element : elements)
    {
        if (!element->Match(p))
            return false;
    }
    return true;
}

void
Filter::AddFilterElement(Ptr<FilterElement> filterElement)
{
    elements.push_back(filterElement);
}

} // namespace ns3
