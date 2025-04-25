#include "filter-element.h"

#include "ns3/log.h"

namespace ns3
{

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
                                          MakeIntegerAccessor(&SourcePortNumber::value),
                                          MakeIntegerChecker<uint32_t>());
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
                                          MakeIntegerAccessor(&DestinationPortNumber::value),
                                          MakeIntegerChecker<uint32_t>());
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
                                          MakeIntegerAccessor(&ProtocolNumber::value),
                                          MakeIntegerChecker<uint32_t>());
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
bool
SourceIpAddress::Match(Ptr<Packet> p) const
{
    Ipv4Header header;
    if (p->PeekHeader(header))
    {
        return header.GetSource() == value;
    }
    return false;
}

bool
SourceMask::Match(Ptr<Packet> p) const
{
    Ipv4Header header;
    if (p->PeekHeader(header))
    {
        Ipv4Address src = header.GetSource();
        return src.CombineMask(value) == addr.CombineMask(value);
    }
    return false;
}

bool
SourcePortNumber::Match(Ptr<Packet> p) const
{
    UdpHeader udp;
    TcpHeader tcp;

    if (p->PeekHeader(udp))
    {
        return udp.GetSourcePort() == value;
    }
    else if (p->PeekHeader(tcp))
    {
        return tcp.GetSourcePort() == value;
    }
    return false;
}

bool
DestinationIpAddress::Match(Ptr<Packet> p) const
{
    Ipv4Header header;
    if (p->PeekHeader(header))
    {
        return header.GetDestination() == value;
    }
    return false;
}

bool
DestinationMask::Match(Ptr<Packet> p) const
{
    Ipv4Header header;
    if (p->PeekHeader(header))
    {
        Ipv4Address dst = header.GetDestination();
        return dst.CombineMask(value) == addr.CombineMask(value);
    }
    return false;
}

bool
DestinationPortNumber::Match(Ptr<Packet> p) const
{
    UdpHeader udp;
    TcpHeader tcp;

    if (p->PeekHeader(udp))
    {
        return udp.GetDestinationPort() == value;
    }
    else if (p->PeekHeader(tcp))
    {
        return tcp.GetDestinationPort() == value;
    }
    return false;
}

bool
ProtocolNumber::Match(Ptr<Packet> p) const
{
    Ipv4Header header;
    if (p->PeekHeader(header))
    {
        return header.GetProtocol() == value;
    }
    return false;
}

} // namespace ns3
