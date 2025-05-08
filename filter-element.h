/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai <kdai3@dons.usfca.edu>, Tiansi Gu <tgu10@dons.usfca.edu>
 */

#ifndef FILTER_ELEMENT_H
#define FILTER_ELEMENT_H

#include "ns3/internet-module.h"
#include "ns3/object.h"

namespace ns3
{

class FilterElement : public Object
{
  public:
    /* Virtual Destructor - open to extension */
    virtual ~FilterElement() = default;

    /* Mark the method as not changing member variables and abstract (0) */
    virtual bool Match(ns3::Ptr<ns3::Packet> p) const = 0;
};

class SourceIpAddress : public FilterElement
{
  private:
    ns3::Ipv4Address value;

  public:
    static TypeId GetTypeId();

    SourceIpAddress();

    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

class SourceMask : public FilterElement
{
  private:
    ns3::Ipv4Mask value;
    ns3::Ipv4Address addr;

  public:
    static TypeId GetTypeId();

    SourceMask();

    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

class SourcePortNumber : public FilterElement
{
  private:
    uint32_t value;

  public:
    static TypeId GetTypeId();

    SourcePortNumber();

    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

class DestinationIpAddress : public FilterElement
{
  private:
    ns3::Ipv4Address value;

  public:
    static TypeId GetTypeId();

    DestinationIpAddress();

    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

class DestinationMask : public FilterElement
{
  private:
    ns3::Ipv4Mask value;
    ns3::Ipv4Address addr;

  public:
    static TypeId GetTypeId();

    DestinationMask();

    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

class DestinationPortNumber : public FilterElement
{
  private:
    uint32_t value;

  public:
    static TypeId GetTypeId();

    DestinationPortNumber();

    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

class ProtocolNumber : public FilterElement
{
  private:
    uint32_t value;

  public:
    static TypeId GetTypeId();

    ProtocolNumber();

    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

} // namespace ns3

#endif // FILTER_ELEMENT_H
