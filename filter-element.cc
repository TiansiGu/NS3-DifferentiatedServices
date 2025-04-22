#ifndef FILTER_ELEMENT_H
#define FILTER_ELEMENT_H

#include "ns3/packet.h"
#include "ns3/ptr.h"

class FilterElement
{
  public:
    /* Virtual Destructor */
    virtual ~FilterElement() = default;

    /* Mark the method as not changing member variables and abstract */
    virtual bool match(ns3::Ptr<ns3::Packet> p) const = 0;
};

#endif // FILTER_ELEMENT_H
