/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai, Tiansi Gu <tgu10@dons.usfca.edu>
 */

#ifndef FILTER_ELEMENT_H
#define FILTER_ELEMENT_H

#include "ns3/internet-module.h"

namespace ns3
{

class FilterElement
{
  public:
    /* Virtual Destructor - open to extension */
    virtual ~FilterElement() = default;

    /* Mark the method as not changing member variables and abstract (0) */
    virtual bool Match(ns3::Ptr<ns3::Packet> p) const = 0;
};

class SourceIPAddress : public FilterElement
{
  private:
    ns3::Ipv4Address value;

  public:
    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

class SourceMask : public FilterElement
{
  private:
    ns3::Ipv4Mask value;
    ns3::Ipv4Address addr;

  public:
    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

class SourcePortNumber : public FilterElement
{
  private:
    uint32_t value;

  public:
    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

class DestinationIPAddress : public FilterElement
{
  private:
    ns3::Ipv4Address value;

  public:
    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

class DestinationMask : public FilterElement
{
  private:
    ns3::Ipv4Mask value;
    ns3::Ipv4Address addr;

  public:
    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

class DestinationPortNumber : public FilterElement
{
  private:
    uint32_t value;

  public:
    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

class ProtocolNumber : public FilterElement
{
  private:
    uint32_t value;

  public:
    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

} // namespace ns3

#endif // FILTER_ELEMENT_H
