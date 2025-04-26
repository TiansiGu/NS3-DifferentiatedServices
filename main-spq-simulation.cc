
#include "spq.h"

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/queue.h"
#include "ns3/traffic-control-module.h"
// #include "creators/qos-initializer.h"
#include "qos-initializer.h"

#include <iostream>

using namespace ns3;

// Global containers
NodeContainer nodes;
NetDeviceContainer dev01, dev12;
Ipv4InterfaceContainer if01, if12;

/**
 * @brief Setup 3-node topology and attach SPQ queue at the router
 *
 * Topology:
 *  node0 --- node1 --- node2
 *  sender    router    receiver
 */
void
SetupSpqTopology(std::string configFile)
{
    nodes.Create(3);

    PointToPointHelper p2p01, p2p12;
    p2p01.SetDeviceAttribute("DataRate", StringValue("4Mbps"));
    p2p01.SetChannelAttribute("Delay", StringValue("2ms"));
    p2p12.SetDeviceAttribute("DataRate", StringValue("1Mbps"));
    p2p12.SetChannelAttribute("Delay", StringValue("2ms"));

    dev01 = p2p01.Install(nodes.Get(0), nodes.Get(1));
    dev12 = p2p12.Install(nodes.Get(1), nodes.Get(2));

    InternetStackHelper stack;
    stack.InstallAll();

    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.0.0.0", "255.255.255.0");
    if01 = ipv4.Assign(dev01);

    ipv4.SetBase("10.0.1.0", "255.255.255.0");
    if12 = ipv4.Assign(dev12);

    // Create SPQ queue from config file and attach to router's outgoing device
    // Ptr<Queue<Packet>> spqQueue = CreateSpqQueueFromJson(configFile);
    // Ptr<PointToPointNetDevice> routerDev = dev12.Get(0)->GetObject<PointToPointNetDevice>();
    // routerDev->SetQueue(spqQueue);

    //  New: Set router's outgoing NetDevice to use SPQ
    p2p12.SetQueue("ns3::StrictPriorityQueue<Packet>", "Config", StringValue(configFile));

    // Enable packet capture
    p2p01.EnablePcap("spq-node0-node1", dev01, true);
    p2p12.EnablePcap("spq-node1-node2", dev12, true);
}

int
main(int argc, char* argv[])
{
    std::string spqConfig = "spq-config.json";
    double simDuration = 50.0;

    CommandLine cmd;
    cmd.AddValue("spqConfig", "Path to SPQ config JSON", spqConfig);
    cmd.Parse(argc, argv);

    SetupSpqTopology(spqConfig);

    // Install 2 flows for SPQ: A (high priority), B (low priority)
    uint16_t basePort = 7000;

    // Flow B (low priority) — start early
    {
        Address sinkAddr = InetSocketAddress(if12.GetAddress(1), basePort + 1);
        PacketSinkHelper sink("ns3::TcpSocketFactory", sinkAddr);
        ApplicationContainer sinkApp = sink.Install(nodes.Get(2));
        sinkApp.Start(Seconds(0.0));
        sinkApp.Stop(Seconds(50.0));

        BulkSendHelper source("ns3::TcpSocketFactory", sinkAddr);
        source.SetAttribute("MaxBytes", UintegerValue(0));
        source.SetAttribute("SendSize", UintegerValue(1024));
        ApplicationContainer sourceApp = source.Install(nodes.Get(0));
        sourceApp.Start(Seconds(0.0));
        sourceApp.Stop(Seconds(50.0));
    }

    // Flow A (high priority) — starts later to test preemption
    {
        Address sinkAddr = InetSocketAddress(if12.GetAddress(1), basePort + 0);
        PacketSinkHelper sink("ns3::TcpSocketFactory", sinkAddr);
        ApplicationContainer sinkApp = sink.Install(nodes.Get(2));
        sinkApp.Start(Seconds(0.0));
        sinkApp.Stop(Seconds(50.0));

        BulkSendHelper source("ns3::TcpSocketFactory", sinkAddr);
        source.SetAttribute("MaxBytes", UintegerValue(0));
        source.SetAttribute("SendSize", UintegerValue(1024));
        ApplicationContainer sourceApp = source.Install(nodes.Get(0));
        sourceApp.Start(Seconds(15.0)); // delay start
        sourceApp.Stop(Seconds(35.0));  // ends early
    }

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    Simulator::Stop(Seconds(simDuration));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}