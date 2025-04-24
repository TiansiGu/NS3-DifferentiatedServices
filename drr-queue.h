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
     DrrQueue(std::vector<uint32_t> quantumValues);
 
     Ptr<Packet> Schedule() override;
 
     uint32_t Classify(Ptr<Packet> p) override;
 
     Ptr<const Packet> Peek() const override;
 
   private:
     std::vector<uint32_t> m_quantums;         
     std::vector<uint32_t> m_deficitCounters;  
     uint32_t m_currentIndex;                  
 };
 
 } // namespace ns3
 
 #endif // DRR_QUEUE_H
 