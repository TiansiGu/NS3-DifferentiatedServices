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

namespace ns3
{

class Filter
{
  private:
    std::vector<FilterElement*> elements;

  public:
    bool match(ns3::Ptr<ns3::Packet> p) const;
};

} // namespace ns3

#endif // FILTER_H
