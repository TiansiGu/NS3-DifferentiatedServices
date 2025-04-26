/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai, Tiansi Gu <tgu10@dons.usfca.edu>
 */

#ifndef FILTER_H
#define FILTER_H

#include "filter-element.h"

#include "ns3/internet-module.h"
#include "ns3/object.h"

namespace ns3
{

class Filter : public Object
{
  private:
    std::vector<Ptr<FilterElement>> elements;

  public:
    static TypeId GetTypeId();

    Filter();

    bool Match(ns3::Ptr<ns3::Packet> p) const;

    void AddFilterElement(Ptr<FilterElement> filterElement);
};

} // namespace ns3

#endif // FILTER_H
