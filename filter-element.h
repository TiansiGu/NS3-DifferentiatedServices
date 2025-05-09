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

/**
 * @brief Abstract base class representing a single filter condition.
 *
 * Subclasses implement specific matching logic on a packet (e.g., source/destination IP, port
 * number).
 */
class FilterElement : public Object
{
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~FilterElement() = default;

    /**
     * @brief Checks if a packet matches this filter element.
     * @param p The packet to test.
     * @return true if the packet matches; false otherwise.
     */
    virtual bool Match(ns3::Ptr<ns3::Packet> p) const = 0;
};

/**
 * @brief Matches packets by exact source IP address.
 */
class SourceIpAddress : public FilterElement
{
  private:
    ns3::Ipv4Address value; //!< Source IP to match

  public:
    static TypeId GetTypeId();

    SourceIpAddress();

    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

/**
 * @brief Matches packets by masked source IP address (e.g., subnet).
 */
class SourceMask : public FilterElement
{
  private:
    ns3::Ipv4Mask value;   //!< Netmask
    ns3::Ipv4Address addr; //!< Base IP address of subnet

  public:
    static TypeId GetTypeId();

    SourceMask();

    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

/**
 * @brief Matches packets by source port number (TCP or UDP).
 */
class SourcePortNumber : public FilterElement
{
  private:
    uint32_t value; //!< Source port to match

  public:
    static TypeId GetTypeId();

    SourcePortNumber();

    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

/**
 * @brief Matches packets by exact destination IP address.
 */
class DestinationIpAddress : public FilterElement
{
  private:
    ns3::Ipv4Address value; //!< Destination IP to match

  public:
    static TypeId GetTypeId();

    DestinationIpAddress();

    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

/**
 * @brief Matches packets by masked destination IP address (e.g., subnet).
 */
class DestinationMask : public FilterElement
{
  private:
    ns3::Ipv4Mask value;   //!< Netmask
    ns3::Ipv4Address addr; //!< Base IP address of subnet

  public:
    static TypeId GetTypeId();

    DestinationMask();

    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

/**
 * @brief Matches packets by destination port number (TCP or UDP).
 */
class DestinationPortNumber : public FilterElement
{
  private:
    uint32_t value; //!< Destination port to match

  public:
    static TypeId GetTypeId();

    DestinationPortNumber();

    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

/**
 * @brief Matches packets by transport protocol (e.g., TCP = 6, UDP = 17).
 */
class ProtocolNumber : public FilterElement
{
  private:
    uint32_t value; //!< IP protocol number

  public:
    static TypeId GetTypeId();

    ProtocolNumber();

    bool Match(ns3::Ptr<ns3::Packet> p) const override;
};

} // namespace ns3

#endif // FILTER_ELEMENT_H
