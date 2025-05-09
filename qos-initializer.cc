/*
 * Copyright (c) YEAR COPYRIGHTHOLDER
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Kexin Dai <kdai3@dons.usfca.edu>, Tiansi Gu <tgu10@dons.usfca.edu>
 */

#include "qos-initializer.h"

#include "./spq.h"
#include "json.hpp"

#include "ns3/log.h"
#include "ns3/string.h"

#include <fstream>

// Use nlohmann JSON library
using json = nlohmann::json;

namespace ns3
{

static nlohmann::json LoadJson(const std::string& filepath);
static Ptr<Filter> CreateFilter(const json& filterConf);
static Ptr<FilterElement> CreateFilterElement(const json& filterElementConf);
static Ipv4Mask MakeIpv4MaskFromPrefixLength(uint8_t prefixLength);

/**
 * @brief Initialize a StrictPriorityQueue instance from a JSON config file.
 *
 * This method parses the JSON config file and dynamically creates multiple TrafficClass
 * instances with filters and priority levels, then adds them to the SPQ scheduler.
 *
 * @param spq Pointer to the StrictPriorityQueue to be configured.
 * @param filepath Path to the JSON configuration file.
 */
void
QosInitializer::InitializeSpqFromJson(Ptr<StrictPriorityQueue> spq, const std::string& filepath)
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
        tcFactory.Set("priority_level", UintegerValue(priorityLevelJson.get<uint32_t>()));

        Ptr<TrafficClass> tc = DynamicCast<TrafficClass>(tcFactory.Create());

        for (const auto& filterConf : queueConf["filters"])
        {
            Ptr<Filter> filter = CreateFilter(filterConf);
            tc->AddFilter(filter);
        }

        spq->AddTrafficClass(tc);
    }
}

/**
 * @brief Initialize a DrrQueue instance from a JSON config file.
 *
 * Each TrafficClass is created with a weight, filters, and maxPackets value.
 *
 * @param drr Pointer to the DrrQueue to be configured.
 * @param filepath Path to the JSON configuration file.
 */
void
QosInitializer::InitializeDrrFromJson(Ptr<DrrQueue> drr, const std::string& filepath)
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
        const auto& weightJson = queueConf["weight"];
        tcFactory.Set("weight", UintegerValue(weightJson.get<uint32_t>()));

        Ptr<TrafficClass> tc = DynamicCast<TrafficClass>(tcFactory.Create());

        for (const auto& filterConf : queueConf["filters"])
        {
            Ptr<Filter> filter = CreateFilter(filterConf);
            tc->AddFilter(filter);
        }

        drr->AddTrafficClass(tc);
    }
}

/**
 * @brief Construct a Filter object from a list of FilterElements.
 *
 * Each Filter is composed of one or more FilterElements (e.g., match on IP or port).
 *
 * @param filterConf The JSON array representing a filter (AND group of elements).
 * @return Ptr<Filter> The constructed Filter object.
 */
static Ptr<Filter>
CreateFilter(const json& filterConf)
{
    ObjectFactory filterFactory;
    filterFactory.SetTypeId("ns3::Filter");

    Ptr<Filter> filter = DynamicCast<Filter>(filterFactory.Create());

    for (const auto& filterElementConf : filterConf)
    {
        Ptr<FilterElement> filterElement = CreateFilterElement(filterElementConf);
        filter->AddFilterElement(filterElement);
    }

    return filter;
}

/**
 * @brief Create a FilterElement based on the "type" field from JSON.
 *
 * This function supports matching on IP addresses, port numbers, masks, and protocol numbers.
 *
 * @param filterElementConf JSON object describing one matching condition.
 * @return Ptr<FilterElement> A fully constructed filter element.
 */
static Ptr<FilterElement>
CreateFilterElement(const json& filterElementConf)
{
    ObjectFactory feFactory;

    const std::string& type = filterElementConf["type"].get<std::string>();
    feFactory.SetTypeId("ns3::" + type);

    const auto& valueJson = filterElementConf["value"];
    if (type == "SourceIpAddress" || type == "DestinationIpAddress")
    {
        Ipv4Address addr = Ipv4Address(valueJson.get<std::string>().c_str());
        feFactory.Set("value", Ipv4AddressValue(addr));
    }
    else if (type == "SourceMask" || type == "DestinationMask")
    {
        const auto& addrJson = filterElementConf["addr"];
        Ipv4Address addr = Ipv4Address(addrJson.get<std::string>().c_str());
        feFactory.Set("addr", Ipv4AddressValue(addr));
        Ipv4Mask mask = MakeIpv4MaskFromPrefixLength(valueJson.get<uint8_t>());
        feFactory.Set("value", Ipv4MaskValue(mask));
    }
    else if (type == "SourcePortNumber" || type == "DestinationPortNumber" ||
             type == "ProtocolNumber")
    {
        feFactory.Set("value", UintegerValue(valueJson.get<uint32_t>()));
    }

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
