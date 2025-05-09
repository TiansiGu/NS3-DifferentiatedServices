/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai <kdai3@dons.usfca.edu>, Tiansi Gu <tgu10@dons.usfca.edu>
 */

#ifndef FILTER_H
#define FILTER_H

#include "filter-element.h"

#include "ns3/internet-module.h"
#include "ns3/object.h"

namespace ns3
{

/**
 * @brief Represents a logical conjunction (AND) of multiple FilterElements.
 *
 * A packet matches this Filter only if it satisfies all FilterElement conditions.
 */
class Filter : public Object
{
  private:
    std::vector<Ptr<FilterElement>> elements; //!< List of conditions that must all match

  public:
    /**
     * @brief Register this class with the ns-3 type system.
     *
     * @return TypeId associated with this class.
     */
    static TypeId GetTypeId();

    /**
     * @brief Default constructor.
     */
    Filter();

    /**
     * @brief Check whether a packet matches all FilterElement conditions in this filter.
     *
     * @param p Packet to test against the filter.
     * @return true if all conditions are satisfied; false otherwise.
     */
    bool Match(ns3::Ptr<ns3::Packet> p) const;

    /**
     * @brief Add a new FilterElement to this filter.
     *
     * @param filterElement The FilterElement to add.
     */
    void AddFilterElement(Ptr<FilterElement> filterElement);
};

} // namespace ns3

#endif // FILTER_H
