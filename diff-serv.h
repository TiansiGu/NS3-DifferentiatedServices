/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai, Tiansi Gu <tgu10@dons.usfca.edu>
 */

#ifndef DIFF_SERV_H
#define DIFF_SERV_H

#include "traffic-class.h"

#include "ns3/queue.h"

namespace ns3
{

class DiffServ : public Queue<Packet>
{
  private:
    std::vector<Ptr<TrafficClass>> q_class; // a collection of Traffic Class

  public:
    bool Enqueue(Ptr<Packet> p) override;

    Ptr<Packet> Dequeue() override;

    Ptr<Packet> Remove() override;

    Ptr<const Packet> Peek() const override;

    virtual Ptr<Packet> Schedule() = 0; // abstract method

    virtual uint32_t Classify(Ptr<Packet> p) = 0; // abstract method

    virtual void AddTrafficClass(Ptr<TrafficClass> trafficClass);

  protected:
    std::vector<Ptr<TrafficClass>>& GetTrafficClasses();             // for write
    const std::vector<Ptr<TrafficClass>>& GetTrafficClasses() const; // for read
};

} // namespace ns3

#endif // DIFF_SERV_H
