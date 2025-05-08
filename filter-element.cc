#include "filter-element.h"

#include "ns3/log.h"
#include "ns3/ppp-header.h"

namespace ns3
{
// NS_OBJECT_ENSURE_REGISTERED(FilterElement);
NS_OBJECT_ENSURE_REGISTERED(SourceIpAddress);
NS_OBJECT_ENSURE_REGISTERED(DestinationIpAddress);
NS_OBJECT_ENSURE_REGISTERED(SourceMask);
NS_OBJECT_ENSURE_REGISTERED(DestinationMask);
NS_OBJECT_ENSURE_REGISTERED(SourcePortNumber);
NS_OBJECT_ENSURE_REGISTERED(DestinationPortNumber);
NS_OBJECT_ENSURE_REGISTERED(ProtocolNumber);

/* Generate log component */
NS_LOG_COMPONENT_DEFINE("FilterElement");

TypeId
SourceIpAddress::GetTypeId()
{
    static TypeId tid = TypeId("ns3::SourceIpAddress")
                            .SetParent<FilterElement>()
                            .AddConstructor<SourceIpAddress>()
                            // Register ip address
                            .AddAttribute("value",
                                          "The source ip address to match.",
                                          Ipv4AddressValue(),
                                          MakeIpv4AddressAccessor(&SourceIpAddress::value),
                                          MakeIpv4AddressChecker());
    ;
    return tid;
}

TypeId
DestinationIpAddress::GetTypeId()
{
    static TypeId tid = TypeId("ns3::DestinationIpAddress")
                            .SetParent<FilterElement>()
                            .AddConstructor<DestinationIpAddress>()
                            // Register ip address
                            .AddAttribute("value",
                                          "The destination ip address to match.",
                                          Ipv4AddressValue(),
                                          MakeIpv4AddressAccessor(&DestinationIpAddress::value),
                                          MakeIpv4AddressChecker());
    ;
    return tid;
}

TypeId
SourceMask::GetTypeId()
{
    static TypeId tid = TypeId("ns3::SourceMask")
                            .SetParent<FilterElement>()
                            .AddConstructor<SourceMask>()
                            // Register ip addr
                            .AddAttribute("addr",
                                          "The start address of source ip address subnet block.",
                                          Ipv4AddressValue(),
                                          MakeIpv4AddressAccessor(&SourceMask::addr),
                                          MakeIpv4AddressChecker())
                            // Register mask value
                            .AddAttribute("value",
                                          "The mask of source ip address range.",
                                          Ipv4MaskValue(),
                                          MakeIpv4MaskAccessor(&SourceMask::value),
                                          MakeIpv4MaskChecker());

    return tid;
}

TypeId
DestinationMask::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::DestinationMask")
            .SetParent<FilterElement>()
            .AddConstructor<DestinationMask>()
            // Register ip addr
            .AddAttribute("addr",
                          "The start address of destination ip address subnet block.",
                          Ipv4AddressValue(),
                          MakeIpv4AddressAccessor(&DestinationMask::addr),
                          MakeIpv4AddressChecker())
            // Register mask value
            .AddAttribute("value",
                          "The mask of source ip address range.",
                          Ipv4MaskValue(),
                          MakeIpv4MaskAccessor(&DestinationMask::value),
                          MakeIpv4MaskChecker());

    return tid;
}

TypeId
SourcePortNumber::GetTypeId()
{
    static TypeId tid = TypeId("ns3::SourcePortNumber")
                            .SetParent<FilterElement>()
                            .AddConstructor<SourcePortNumber>()
                            // Register port number
                            .AddAttribute("value",
                                          "The source port number to match.",
                                          UintegerValue(),
                                          MakeUintegerAccessor(&SourcePortNumber::value),
                                          MakeUintegerChecker<uint32_t>());
    return tid;
}

TypeId
DestinationPortNumber::GetTypeId()
{
    static TypeId tid = TypeId("ns3::DestinationPortNumber")
                            .SetParent<FilterElement>()
                            .AddConstructor<DestinationPortNumber>()
                            // Register port number
                            .AddAttribute("value",
                                          "The destination port number to match.",
                                          UintegerValue(),
                                          MakeUintegerAccessor(&DestinationPortNumber::value),
                                          MakeUintegerChecker<uint32_t>());
    return tid;
}

TypeId
ProtocolNumber::GetTypeId()
{
    static TypeId tid = TypeId("ns3::ProtocolNumber")
                            .SetParent<FilterElement>()
                            .AddConstructor<ProtocolNumber>()
                            // Register protocol type number
                            .AddAttribute("value",
                                          "The destination port number to match.",
                                          UintegerValue(),
                                          MakeUintegerAccessor(&ProtocolNumber::value),
                                          MakeUintegerChecker<uint32_t>());
    return tid;
}

/* Empty constructors */
SourceIpAddress::SourceIpAddress()
{
    NS_LOG_FUNCTION(this);
}

SourceMask::SourceMask()
{
    NS_LOG_FUNCTION(this);
}

SourcePortNumber::SourcePortNumber()
{
    NS_LOG_FUNCTION(this);
}

DestinationIpAddress::DestinationIpAddress()
{
    NS_LOG_FUNCTION(this);
}

DestinationMask::DestinationMask()
{
    NS_LOG_FUNCTION(this);
}

DestinationPortNumber::DestinationPortNumber()
{
    NS_LOG_FUNCTION(this);
}

ProtocolNumber::ProtocolNumber()
{
    NS_LOG_FUNCTION(this);
}

/* Method Implementations*/
/**
 * @brief Match packets by exact source IP address.
 */
bool
SourceIpAddress::Match(Ptr<Packet> p) const
{

    // Remove PPP header
    Ptr<Packet> pCopy = p->Copy();
    PppHeader pppHeader;
    if (pCopy->RemoveHeader(pppHeader) == 0)
    {
        NS_LOG_ERROR("Failed to remove PPP header from packet");
        return false;
    }

    Ipv4Header header;
    if (pCopy->PeekHeader(header))
    {
        // NS_LOG_UNCOND("IP address value: " << value);
        // NS_LOG_UNCOND("IP address: " << header.GetSource());
        return header.GetSource() == value;
    }
    return false;
}

/**
 * @brief Match packets whose source IP falls within a given subnet.
 */
bool
SourceMask::Match(Ptr<Packet> p) const
{
    // Remove PPP header
    Ptr<Packet> pCopy = p->Copy();
    PppHeader pppHeader;
    if (pCopy->RemoveHeader(pppHeader) == 0)
    {
        NS_LOG_ERROR("Failed to remove PPP header from packet");
        return false;
    }

    Ipv4Header header;
    if (pCopy->PeekHeader(header))
    {
        Ipv4Address src = header.GetSource();
        return src.CombineMask(value) == addr.CombineMask(value);
    }
    return false;
}

/**
 * @brief Match packets by source port number (UDP or TCP).
 */
bool
SourcePortNumber::Match(Ptr<Packet> p) const
{
    // Remove PPP header and IP header
    Ptr<Packet> pCopy = p->Copy();

    PppHeader pppHeader;
    Ipv4Header ipHeader;
    if (pCopy->RemoveHeader(pppHeader) == 0)
    {
        NS_LOG_ERROR("Failed to remove PPP header from packet");
        return false;
    }
    if (pCopy->RemoveHeader(ipHeader) == 0)
    {
        NS_LOG_ERROR("Failed to remove IP header from packet");
        return false;
    }

    UdpHeader udp;
    TcpHeader tcp;

    if (pCopy->PeekHeader(udp))
    {
        return udp.GetSourcePort() == value;
    }
    else if (pCopy->PeekHeader(tcp))
    {
        return tcp.GetSourcePort() == value;
    }
    return false;
}

/**
 * @brief Match packets by exact destination IP address.
 */
bool
DestinationIpAddress::Match(Ptr<Packet> p) const
{
    // Remove PPP header
    Ptr<Packet> pCopy = p->Copy();
    PppHeader pppHeader;
    if (pCopy->RemoveHeader(pppHeader) == 0)
    {
        NS_LOG_ERROR("Failed to remove PPP header from packet");
        return false;
    }

    Ipv4Header header;
    if (pCopy->PeekHeader(header))
    {
        return header.GetDestination() == value;
    }
    return false;
}

/**
 * @brief Match packets whose destination IP falls within a given subnet.
 */
bool
DestinationMask::Match(Ptr<Packet> p) const
{
    // Remove PPP header
    Ptr<Packet> pCopy = p->Copy();
    PppHeader pppHeader;
    if (pCopy->RemoveHeader(pppHeader) == 0)
    {
        NS_LOG_ERROR("Failed to remove PPP header from packet");
        return false;
    }

    Ipv4Header header;
    if (pCopy->PeekHeader(header))
    {
        Ipv4Address dst = header.GetDestination();
        return dst.CombineMask(value) == addr.CombineMask(value);
    }
    return false;
}

/**
 * @brief Match packets by destination port number (UDP or TCP).
 */
bool
DestinationPortNumber::Match(Ptr<Packet> p) const
{
    // Remove PPP header and IP header
    Ptr<Packet> pCopy = p->Copy();

    PppHeader pppHeader;
    Ipv4Header ipHeader;
    if (pCopy->RemoveHeader(pppHeader) == 0)
    {
        NS_LOG_ERROR("Failed to remove PPP header from packet");
        NS_LOG_UNCOND("Remove PPP header error");
        return false;
    }
    if (pCopy->RemoveHeader(ipHeader) == 0)
    {
        NS_LOG_ERROR("Failed to remove IP header from packet");
        NS_LOG_UNCOND("Remove Ip header error");
        return false;
    }

    UdpHeader udp;
    TcpHeader tcp;

    if (pCopy->PeekHeader(udp))
    {

        return udp.GetDestinationPort() == value;
    }
    else if (pCopy->PeekHeader(tcp))
    {
        return tcp.GetDestinationPort() == value;
    }
    return false;

}

/**
 * @brief Match packets by IP protocol number (e.g., TCP=6, UDP=17).
 */
bool
ProtocolNumber::Match(Ptr<Packet> p) const
{
    // Remove PPP header
    Ptr<Packet> pCopy = p->Copy();
    PppHeader pppHeader;
    if (pCopy->RemoveHeader(pppHeader) == 0)
    {
        NS_LOG_ERROR("Failed to remove PPP header from packet");
        return false;
    }

    Ipv4Header header;
    if (pCopy->PeekHeader(header))
    {
        return header.GetProtocol() == value;
    }
    return false;
}

} // namespace ns3
