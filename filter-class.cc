/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai <kdai3@dons.usfca.edu>, Tiansi Gu <tgu10@dons.usfca.edu>
 */

#include "filter-class.h"

namespace ns3
{
// Register Filter as an ns-3 object with runtime type information
NS_OBJECT_ENSURE_REGISTERED(Filter);

/**
 * @brief Registers the Filter class with the ns-3 type system.
 *
 * @return TypeId of the Filter class.
 */
TypeId
Filter::GetTypeId()
{
    static TypeId tid = TypeId("ns3::Filter").SetParent<Object>().AddConstructor<Filter>();
    return tid;
}

Filter::Filter()
{
}

/**
 * @brief Evaluates whether a packet matches all filter elements.
 *
 * This method implements a logical AND over all FilterElement instances
 * in the filter. The packet must match all conditions to pass.
 *
 * @param p The packet to test against the filter.
 * @return true if all FilterElement conditions are satisfied, false otherwise.
 */
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

/**
 * @brief Adds a FilterElement (a basic matching condition) to this filter.
 *
 * @param filterElement The FilterElement to be added.
 */
void
Filter::AddFilterElement(Ptr<FilterElement> filterElement)
{
    elements.push_back(filterElement);
}

} // namespace ns3
