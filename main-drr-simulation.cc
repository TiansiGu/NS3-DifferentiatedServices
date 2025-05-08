
#include "drr-queue.h"
#include "qos-initializer.h"

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/queue.h"
#include "ns3/traffic-control-module.h"

#include <iostream>

using namespace ns3;

// NS_LOG_COMPONENT_DEFINE("MainDrrSimulation");

// Global containers
NodeContainer nodes;
NetDeviceContainer dev01, dev12;
Ipv4InterfaceContainer if01, if12;

/**
 * @brief Set up a 3-node topology and attach a DRR queue to the router
 *
 * Topology:
 *   node0 --- node1 --- node2
 *   sender    router    receiver
 *
 * The router's outgoing link uses a DRR queue, initialized from JSON config.
 */
void
SetupDrrTopology(std::string configFile)
{
    NS_LOG_UNCOND("Start to set topology...");
    nodes.Create(3);

    PointToPointHelper p2p01, p2p12;
    p2p01.SetDeviceAttribute("DataRate", StringValue("4Mbps"));
    p2p01.SetChannelAttribute("Delay", StringValue("2ms"));
    p2p12.SetDeviceAttribute("DataRate", StringValue("1Mbps"));
    p2p12.SetChannelAttribute("Delay", StringValue("2ms"));

    //  New: Set router's outgoing NetDevice to use Drr
    p2p12.SetQueue("ns3::DrrQueue<Packet>", "Config", StringValue(configFile));

    dev01 = p2p01.Install(nodes.Get(0), nodes.Get(1));
    dev12 = p2p12.Install(nodes.Get(1), nodes.Get(2));

    Ptr<PointToPointNetDevice> routerDev = dev12.Get(0)->GetObject<PointToPointNetDevice>();

    Ptr<Queue<Packet>> queue = routerDev->GetQueue();
    Ptr<DrrQueue> drr = DynamicCast<DrrQueue>(queue);
    drr->Initialize();

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

    // p2p12.SetQueue("ns3::StrictPriorityQueue<Packet>", "Config", StringValue(configFile));

    // Enable packet capture
    p2p01.EnablePcap("drr-node0-node1", dev01, true);
    p2p12.EnablePcap("drr-node1-node2", dev12, true);
}

int
main(int argc, char* argv[])
{
    std::string drrConfig = "/home/kexin/ns-3-dev/scratch/NS3-DifferentiatedServices/drr.json";
    double simDuration = 50.0;

    // Parse CLI argument: --drrConfig=xxx.json
    CommandLine cmd;
    cmd.AddValue("drrConfig", "Path to DRR config JSON", drrConfig);
    cmd.Parse(argc, argv);

    SetupDrrTopology(drrConfig);

    // Install 3 BulkSend flows: from node0 to node2 on different ports
    uint16_t basePort = 5000;
    for (int i = 0; i < 3; ++i)
    {
        /////////////////////////////////////////
        uint16_t portI = basePort + i;
        UdpServerHelper sink(portI);
        ApplicationContainer sinkApp = sink.Install(nodes.Get(2));
        sinkApp.Start(Seconds(0.0));
        sinkApp.Stop(Seconds(50.0));

        UdpClientHelper source(if12.GetAddress(1), portI);
        source.SetAttribute("MaxPackets", UintegerValue(0));        // infinite packets
        source.SetAttribute("Interval", TimeValue(Seconds(0.004))); // 0.004s interval for 2Mbps
        source.SetAttribute("PacketSize", UintegerValue(1000));     // 1000 bytes per packet
        ApplicationContainer sourceApp = source.Install(nodes.Get(0));
        sourceApp.Start(Seconds(0.0));
        sourceApp.Stop(Seconds(50.0));
    }

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    Simulator::Stop(Seconds(simDuration));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
