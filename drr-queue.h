/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai, Tiansi Gu
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

    uint32_t Classify(Ptr<Packet> p) override;

  protected:
    void DoInitialize() override;

  private:

    std::vector<uint32_t> m_deficitCounters;
    uint32_t m_currentIndex;

    std::string m_configFile; // <- come from SetAttribute
};

} // namespace ns3

#endif // DRR_QUEUE_H
