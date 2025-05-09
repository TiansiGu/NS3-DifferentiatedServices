/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai <kdai3@dons.usfca.edu>, Tiansi Gu <tgu10@dons.usfca.edu>
 */

#ifndef DIFF_SERV_H
#define DIFF_SERV_H

#include "traffic-class.h"

#include "ns3/queue.h"

namespace ns3
{

/**
 * @brief Abstract base class for DiffServ queuing mechanism.
 *
 * Defines the common interface for scheduling and classifying packets
 * using multiple TrafficClass.
 */
class DiffServ : public Queue<Packet>
{
  private:
    std::vector<Ptr<TrafficClass>> q_class; //!< A collection of Traffic Class

    /**
     * @brief Find the index of the next queue to be scheduled.
     *
     * @return Index of the queue, or -1 if no queue is ready for shceduling.
     */
    virtual int32_t GetQueueForSchedule() const = 0;

  public:
    /**
     * @brief Enqueue a packet into its classified traffic class.
     *
     * @param p Packet to enqueue.
     * @return true if successfully enqueued, false otherwise.
     */
    bool Enqueue(Ptr<Packet> p) override;

    /**
     * @brief Dequeue the next scheduled packet.
     *
     * @return The dequeued packet, or nullptr if all queues are empty.
     */
    Ptr<Packet> Dequeue() override;

    /**
     * @brief Remove the next scheduled packet.
     *
     * @return The removed packet, or nullptr if all queues are empty.
     */
    Ptr<Packet> Remove() override;

    /**
     * @brief Peek at the next packet that should be scheduled without removing it.
     *
     * @return The next packet to be dequeued, or nullptr if none.
     */
    Ptr<const Packet> Peek() const override;

    /**
     * @brief Schedule the next packet to be dequeued.
     *
     * @return The scheduled packet, or nullptr if none.
     */
    virtual Ptr<Packet> Schedule() = 0; // abstract method

    /**
     * @brief Classify a packet to a specific traffic class.
     *
     * @param p The pointer of the packet to classify.
     * @return The index of the matching traffic class in q_class.
     */
    virtual int32_t Classify(Ptr<Packet> p) = 0; // abstract method

    /**
     * @brief Add a new traffic class to the queue set.
     *
     * @param trafficClass Pointer to the TrafficClass to add.
     */
    virtual void AddTrafficClass(Ptr<TrafficClass> trafficClass);

  protected:
    /**
     * @brief Get modifiable reference of q_class to support sorting of traffic classes
     *
     * @return Reference to the traffic class vector.
     */
    std::vector<Ptr<TrafficClass>>& GetTrafficClasses(); // for write

    /**
     * @brief Get read-only reference to traffic classes.
     *
     * @return Const reference to the traffic class vector.
     */
    const std::vector<Ptr<TrafficClass>>& GetTrafficClasses() const; // for read
};

} // namespace ns3

#endif // DIFF_SERV_H
