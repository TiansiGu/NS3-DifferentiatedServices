/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai <kdai3@dons.usfca.edu>, Tiansi Gu <tgu10@dons.usfca.edu>
 */

#ifndef QOS_INITIALIZER
#define QOS_INITIALIZER

#include "./drr-queue.h"
#include "./spq.h"

namespace ns3
{

/**
 * @brief Utility class to initialize QoS queue configurations from JSON files.
 *
 * Provides helper methods to parse and apply structured filter and traffic class
 * configurations to SPQ and DRR queues.
 */
class QosInitializer
{
  public:
    /**
     * @brief Initializes a StrictPriorityQueue using a JSON config file.
     * @param spq Pointer to the SPQ queue object.
     * @param filepath Absolute or relative path to the JSON configuration file.
     */
    static void InitializeSpqFromJson(Ptr<StrictPriorityQueue> spq, const std::string& filepath);

    /**
     * @brief Initializes a DrrQueue using a JSON config file.
     * @param drr Pointer to the DRR queue object.
     * @param filepath Absolute or relative path to the JSON configuration file.
     */
    static void InitializeDrrFromJson(Ptr<DrrQueue> drr, const std::string& filepath);
};
} // namespace ns3

#endif // QOS_INITIALIZER