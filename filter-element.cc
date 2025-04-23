#include "filter-element.h"

namespace ns3
{

bool
SourceIPAddress::Match(Ptr<Packet> p) const
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
DestinationIPAddress::Match(Ptr<Packet> p) const
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
