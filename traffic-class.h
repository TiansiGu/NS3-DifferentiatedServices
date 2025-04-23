/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai, Tiansi Gu <tgu10@dons.usfca.edu>
 */

#ifndef TRAFFIC_CLASS_H
#define TRAFFIC_CLASS_H

#include "filter.h"

namespace ns3
{

class TrafficClass
{
  private:
    uint32_t packets;
    uint32_t maxPackets;
    double_t weight; // applicable if the QoS mechanism uses weights
    uint32_t priorityLevel;
    bool isDefault;                       // whether this queue is served as the default queue
    std::queue<Ptr<ns3::Packet>> m_queue; // the queue that holds packet waiting to be scheduled
    std::vector<Filter*> filters;         // a collection of Filters

  public:
    bool IsDefault() const;

    bool Enqueue(Ptr<ns3::Packet> p);

    Ptr<ns3::Packet> Dequeue();

    bool Match(Ptr<ns3::Packet> p) const;
};

} // namespace ns3

#endif // TRAFFIC_CLASS_H
