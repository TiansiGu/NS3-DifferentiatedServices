/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai, Tiansi Gu <tgu10@dons.usfca.edu>
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
    Ptr<Packet> Schedule() override;

    uint32_t Classify(Ptr<Packet> p) override;
};

} // namespace ns3

#endif // STRICT_PRIORITY_QUEUE_H
