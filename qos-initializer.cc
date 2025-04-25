#include "qos-initializer.h"

// #include "../spq.h"
#include "./spq.h"
#include "json.hpp"

#include "ns3/string.h"

#include <fstream>

using json = nlohmann::json;

namespace ns3
{

static nlohmann::json LoadJson(const std::string& filepath);
static Ptr<Filter> CreateFilter(const nlohmann::json& filterConf);
static Ptr<FilterElement> CreateFilterElement(const nlohmann::json& filterElementConf);
static Ipv4Mask MakeIpv4MaskFromPrefixLength(uint8_t prefixLength);

void
QoSInitializer::InitializeSpqFromJson(Ptr<StrictPriorityQueue> spq, const std::string& filepath)
{
    json config = LoadJson(filepath);

    for (const auto& queueConf : config["queues"])
    {
        ObjectFactory tcFactory;
        tcFactory.SetTypeId("ns3::TrafficClass");

        const auto& maxPacketsJson = queueConf["maxPackets"];
        tcFactory.Set("maxPackets", UintegerValue(maxPacketsJson.get<uint32_t>()));
        const auto& isDefaultJson = queueConf["isDefault"];
        tcFactory.Set("isDefault", BooleanValue(isDefaultJson.get<bool>()));
        const auto& priorityLevelJson = queueConf["priorityLevel"];
        tcFactory.Set("priorityLevel", UintegerValue(priorityLevelJson.get<uint32_t>()));

        // Ptr<TrafficClass> tc = tcFactory.Create();
        Ptr<TrafficClass> tc = DynamicCast<TrafficClass>(tcFactory.Create());

        for (const auto& filterConf : config["filters"])
        {
            Ptr<Filter> filter = CreateFilter(filterConf);
            tc->AddFilter(GetPointer(filter));
        }

        spq->AddTrafficClass(GetPointer(tc));
    }
}

static Ptr<Filter>
CreateFilter(const json& filterConf)
{
    ObjectFactory filterFactory;
    filterFactory.SetTypeId("ns3::Filter");

    // Ptr<Filter> filter = filterFactory.Create();
    Ptr<Filter> filter = DynamicCast<Filter>(filterFactory.Create());

    for (const auto& filterElementConf : filterConf)
    {
        Ptr<FilterElement> filterElement = CreateFilterElement(filterElementConf);
        filter->AddFilterElement(GetPointer(filterElement));
    }

    return filter;
}

static Ptr<FilterElement>
CreateFilterElement(const json& filterElementConf)
{
    ObjectFactory feFactory;

    const std::string& type = filterElementConf["type"].dump();
    feFactory.SetTypeId("ns3::" + type);

    const auto& valueJson = filterElementConf["value"];
    if (type == "SourceIpAddress" || type == "DestinationIpAddress")
    {
        Ipv4Address addr = Ipv4Address(valueJson.dump().c_str());
        feFactory.Set("value", Ipv4AddressValue(addr));
    }
    else if (type == "SourceMask" || type == "DestinationMask")
    {
        const auto& addrJson = filterElementConf["addr"];
        Ipv4Address addr = Ipv4Address(addrJson.dump().c_str());
        feFactory.Set("addr", Ipv4AddressValue(addr));
        Ipv4Mask mask = MakeIpv4MaskFromPrefixLength(valueJson.get<int>());
        feFactory.Set("value", Ipv4MaskValue(mask));
    }
    else if (type == "SourcePortNumber" || type == "DestinationPortNumber" ||
             type == "ProtocolNumber")
    {
        feFactory.Set("value", IntegerValue(valueJson.get<int>()));
    }

    // Ptr<FilterElement> filterElement = feFactory.Create();
    Ptr<FilterElement> filterElement = DynamicCast<FilterElement>(feFactory.Create());

    return filterElement;
}

/** Helper function scoped only in this file, load a json object from filepath */
static json
LoadJson(const std::string& filepath)
{
    std::ifstream f(filepath);
    return json::parse(f);
}

/* Create a 32-bit mask: the leading 'prefixLength' bits are 1, rest are 0 */
static Ipv4Mask
MakeIpv4MaskFromPrefixLength(uint8_t prefixLength)
{
    uint32_t mask = (prefixLength == 0) ? 0 : (~0u << (32 - prefixLength));
    return Ipv4Mask(mask);
}

} // namespace ns3

