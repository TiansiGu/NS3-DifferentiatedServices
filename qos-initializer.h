/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai <kdai3@dons.usfca.edu>, Tiansi Gu <tgu10@dons.usfca.edu>
 */

// #include "../spq.h"
#include "./drr-queue.h"
#include "./spq.h"

namespace ns3
{
class QoSInitializer
{
  public:
    static void InitializeSpqFromJson(Ptr<StrictPriorityQueue> spq, const std::string& filepath);

    static void InitializeDrrFromJson(Ptr<DrrQueue> drr, const std::string& filepath);
};
} // namespace ns3
