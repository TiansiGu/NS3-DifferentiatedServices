/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai <kdai3@dons.usfca.edu>, Tiansi Gu <tgu10@dons.usfca.edu>
 */

#ifndef STRICT_PRIORITY_QUEUE_H
#define STRICT_PRIORITY_QUEUE_H

#include "diff-serv.h"

#include "ns3/queue.h"

namespace ns3
{

class StrictPriorityQueue : public DiffServ
{
  public:
    static TypeId GetTypeId();

    StrictPriorityQueue();

    Ptr<Packet> Schedule() override;

    uint32_t Classify(Ptr<Packet> p) override;

    void AddTrafficClass(Ptr<TrafficClass> trafficClass) override;

  private:
    std::string m_configFile;

  protected:
    void DoInitialize() override;
};

} // namespace ns3

#endif // STRICT_PRIORITY_QUEUE_H
