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

/**
 * @brief A DiffServ-based class implementing the Deficit Round Robin (DRR) queuing algorithm.
 */
class DrrQueue : public DiffServ
{
  public:
    /**
     * @brief Register this class with the ns-3 type system.
     *
     * @return The TypeId associated with this class.
     */
    static TypeId GetTypeId();

    /**
     * @brief Default constructor for DrrQueue.
     */
    DrrQueue();

    /**
     * @brief Schedule the next packet to dequeue using DRR logic.
     *
     * @return The next scheduled packet, or nullptr if no packets are available.
     */
    Ptr<Packet> Schedule() override;

    /**
     * @brief Classify a packet into one of the traffic classes.
     *
     * @param p The packet to classify.
     * @return Index of the matching traffic class, or -1 if none match.
     */
    int32_t Classify(Ptr<Packet> p) override;

  protected:
    /**
     * @brief Initialize the DRR queue and its configuration.
     */
    void DoInitialize() override;

  private:
    mutable std::vector<uint32_t> m_deficitCounters; //!< Deficit counters for each class
    mutable uint32_t m_currentIndex;                 //!< Index of the current class being served

    std::string m_configFile; // <- come from SetAttribute

    /**
     * @brief Get the next eligible traffic class index based on DRR scheduling.
     *
     * @return Index of the selected class, or -1 if all queues are empty or ineligible.
     */
    int32_t GetQueueForSchedule() const override;
};

} // namespace ns3

#endif // DRR_QUEUE_H
