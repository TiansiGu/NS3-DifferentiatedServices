/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai <kdai3@dons.usfca.edu>, Tiansi Gu <tgu10@dons.usfca.edu>
 */

#ifndef DRR_QUEUE_H
#define DRR_QUEUE_H

#include "diff-serv.h"

namespace ns3
{

class DrrQueue : public DiffServ
{
  public:
    static TypeId GetTypeId();

    DrrQueue();

    Ptr<Packet> Schedule() override;

    int32_t Classify(Ptr<Packet> p) override;

  protected:
    void DoInitialize() override;

  private:
    mutable std::vector<uint32_t> m_deficitCounters;
    mutable uint32_t m_currentIndex;

    std::string m_configFile; // <- come from SetAttribute

    int32_t GetQueueForSchedule() const override;
};

} // namespace ns3

#endif // DRR_QUEUE_H
