/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai, Tiansi Gu
 */

// #include "../spq.h"
#include "./spq.h"

namespace ns3
{
class QoSInitializer
{
  public:
    static void InitializeSpqFromJson(Ptr<StrictPriorityQueue> spq, const std::string& filepath);
};
} // namespace ns3

