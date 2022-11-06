#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/trace-helper.h"
#include <string>
#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Task_1_Team_1");

void configurationZero(PointToPointStarHelper star, NodeContainer csmaRightNodes, CsmaHelper csmaright, PointToPointHelper pointToPointStar);
void configurationOne(PointToPointStarHelper star, NodeContainer csmaRightNodes, NodeContainer csmaLeftNodes, Ipv4InterfaceContainer csmaLeftInterfaces);
void configurationTwo(PointToPointStarHelper star, NodeContainer csmaRightNodes, NodeContainer csmaLeftNodes, Ipv4InterfaceContainer csmaLeftInterfaces, Ipv4InterfaceContainer csmaRightInterfaces);

std::string getFileName(int configuration, std::string name) {
    std::string filename = "task1-";
    filename += configuration + '0';
    filename += "-" + name;
    return filename;
}

int main(int argc, char* argv[]) {

    LogComponentEnable("Task_1_Team_1", LOG_LEVEL_INFO);

    int configuration = 0;

    CommandLine cmd(__FILE__);
    cmd.AddValue("configuration", "Inserisci il parametro configuration scegliendo tra 0, 1 o 2", configuration);

    cmd.Parse(argc, argv);

    Ipv4AddressHelper address;

    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", StringValue("25Mbps"));
    csma.SetChannelAttribute("Delay", TimeValue(MicroSeconds(10)));

    NodeContainer csmaNodes;
    csmaNodes.Create(3); // n0, n1, n2

    PointToPointHelper pointToPointStar;
    pointToPointStar.SetDeviceAttribute("DataRate", StringValue("80Mbps"));
    pointToPointStar.SetChannelAttribute("Delay", TimeValue(MicroSeconds(5)));

    PointToPointStarHelper star(4, pointToPointStar); // n3, n4, n5, n6, n7

    NetDeviceContainer csmaDevices;
    csmaDevices = csma.Install(csmaNodes);

    InternetStackHelper stack;
    stack.Install(csmaNodes);
    star.InstallStack(stack);

    address.SetBase("192.128.1.0", "255.255.255.0");
    Ipv4InterfaceContainer csmaInterfaces;
    csmaInterfaces = address.Assign(csmaDevices);
    
    star.AssignIpv4Addresses(Ipv4AddressHelper("10.10.1.0", "255.255.255.240"));

    /* Links configuration */

    PointToPointHelper pointToPoint23; // Link between n2 and n3
    pointToPoint23.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
    pointToPoint23.SetChannelAttribute("Delay", TimeValue(MicroSeconds(15)));

    NodeContainer pointToPoint23Nodes;
    pointToPoint23Nodes.Add(csmaNodes.Get(2));
    pointToPoint23Nodes.Add(star.GetSpokeNode(0));

    NetDeviceContainer pointToPoint23Devices;
    pointToPoint23Devices = pointToPoint23.Install(pointToPoint23Nodes);

    address.SetBase("10.1.1.0", "255.255.255.252");
    Ipv4InterfaceContainer pointToPoint23Interfaces;
    pointToPoint23Interfaces = address.Assign(pointToPoint23Devices);



    PointToPointHelper pointToPointl; // l links
    pointToPointl.SetDeviceAttribute("DataRate", StringValue("80Mbps"));
    pointToPointl.SetChannelAttribute("Delay", TimeValue(MicroSeconds(5)));

    NodeContainer pointToPoint34Nodes; // Link between n3 and n4
    pointToPoint34Nodes.Add(star.GetSpokeNode(0));
    pointToPoint34Nodes.Add(star.GetSpokeNode(1));

    NetDeviceContainer pointToPoint34Devices;
    pointToPoint34Devices = pointToPointl.Install(pointToPoint34Nodes);

    address.SetBase("10.0.1.0", "255.255.255.252");
    Ipv4InterfaceContainer pointToPoint34Interfaces;
    pointToPoint34Interfaces = address.Assign(pointToPoint34Devices);


    NodeContainer pointToPoint47Nodes; // Link between n4 and n7
    pointToPoint47Nodes.Add(star.GetSpokeNode(1));
    pointToPoint47Nodes.Add(star.GetSpokeNode(2));

    NetDeviceContainer pointToPoint47Devices;
    pointToPoint47Devices = pointToPointl.Install(pointToPoint47Nodes);

    address.SetBase("10.0.2.0", "255.255.255.252");
    Ipv4InterfaceContainer pointToPoint47Interfaces;
    pointToPoint47Interfaces = address.Assign(pointToPoint47Devices);


    NodeContainer pointToPoint76Nodes; // Link between n7 and n6
    pointToPoint76Nodes.Add(star.GetSpokeNode(2));
    pointToPoint76Nodes.Add(star.GetSpokeNode(3));

    NetDeviceContainer pointToPoint76Devices;
    pointToPoint76Devices = pointToPointl.Install(pointToPoint76Nodes);

    address.SetBase("10.0.3.0", "255.255.255.252");
    Ipv4InterfaceContainer pointToPoint76Interfaces;
    pointToPoint76Interfaces = address.Assign(pointToPoint76Devices);


    NodeContainer pointToPoint36Nodes; // Link between n3 and n6
    pointToPoint36Nodes.Add(star.GetSpokeNode(0));
    pointToPoint36Nodes.Add(star.GetSpokeNode(3));

    NetDeviceContainer pointToPoint36Devices;
    pointToPoint36Devices = pointToPointl.Install(pointToPoint36Nodes);

    address.SetBase("10.0.4.0", "255.255.255.252");
    Ipv4InterfaceContainer pointToPoint36Interfaces;
    pointToPoint36Interfaces = address.Assign(pointToPoint36Devices);


    /* Right side network */

    CsmaHelper csmaright;
    csmaright.SetChannelAttribute("DataRate", StringValue("30Mbps"));
    csmaright.SetChannelAttribute("Delay", TimeValue(MicroSeconds(20)));

    NodeContainer csmaRightNodes;
    csmaRightNodes.Create(2);

    NetDeviceContainer csmaRightDevices;
    csmaRightDevices = csmaright.Install(csmaRightNodes);

    stack.Install(csmaRightNodes);

    Ptr<CsmaChannel> canale = DynamicCast<CsmaChannel> (csmaRightDevices.Get(0)->GetChannel());

    csmaRightNodes.Add(star.GetSpokeNode(2));
    csmaRightDevices.Add(csmaright.Install(star.GetSpokeNode(2), canale));

    address.SetBase("192.128.2.0", "255.255.255.0");
    Ipv4InterfaceContainer csmaRightInterfaces;
    csmaRightInterfaces = address.Assign(csmaRightDevices);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    csma.EnablePcap(getFileName(configuration, "n0.pcap"), csmaDevices.Get(0), true, true); // n0
    pointToPoint23.EnablePcap(getFileName(configuration, "n3.pcap"), pointToPoint23Devices.Get(1), true, true); // n3
    csmaright.EnablePcap(getFileName(configuration, "n7.pcap"), csmaRightDevices.Get(2), true, true); // n7

    AsciiTraceHelper ascii;

    switch(configuration) {
        case 0:
            pointToPointStar.EnableAscii(getFileName(0, "n5.tr"), star.GetHub()->GetDevice(2), true);
            csmaright.EnableAscii(getFileName(0, "n9.tr"), csmaRightNodes.Get(1)->GetDevice(0), true);
            configurationZero(star, csmaRightNodes, csmaright, pointToPointStar);
            break;
        case 1:
            csma.EnableAscii(getFileName(1, "n0.tr"), csmaNodes.Get(0)->GetDevice(0), true);
            pointToPointStar.EnableAscii(getFileName(1, "n5.tr"), star.GetHub()->GetDevice(2), true);
            csmaright.EnableAscii(getFileName(1, "n8.tr"), csmaRightNodes.Get(0)->GetDevice(0), true);
            csmaright.EnableAscii(getFileName(1, "n9.tr"), csmaRightNodes.Get(1)->GetDevice(0), true);

            configurationOne(star, csmaRightNodes, csmaNodes, csmaInterfaces);
            break;
        case 2:
            pointToPoint23.EnableAscii(getFileName(2, "n2.tr"), pointToPoint23Devices.Get(0), true);
            csmaright.EnableAscii(getFileName(2, "n8.tr"), csmaRightNodes.Get(0)->GetDevice(0), true);

            pointToPointStar.EnableAscii(getFileName(2, "n5.tr"), star.GetHub()->GetDevice(2), true);
            csmaright.EnableAscii(getFileName(2, "n9.tr"), csmaRightNodes.Get(1)->GetDevice(0), true);

            csma.EnableAscii(getFileName(2, "n0.tr"), csmaNodes.Get(0)->GetDevice(0), true);

            configurationTwo(star, csmaRightNodes, csmaNodes, csmaInterfaces, csmaRightInterfaces);
            break;
        default:
            return -1;
    }

    Simulator::Stop(Seconds(20.0));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}

void createSink(uint16_t port, ns3::Ptr<ns3::Node> node, double tinit, double tfinish, bool udp) {
    Address sinkLocalAddress (InetSocketAddress (Ipv4Address::GetAny(), port));
    PacketSinkHelper sinkHelper (udp ? "ns3::UdpSocketFactory" : "ns3::TcpSocketFactory", sinkLocalAddress);
    ApplicationContainer sinkApp = sinkHelper.Install(node); // n5
    sinkApp.Start(Seconds(tinit));
    sinkApp.Stop(Seconds(tfinish));
}

void configurationZero(PointToPointStarHelper star, NodeContainer csmaRightNodes, CsmaHelper csmaright, PointToPointHelper pointToPointStar) {
    /* TCP Sink server */

    uint16_t port = 2300;
    createSink(port, star.GetHub(), 0, 20.0, false);

    // OnOff TCP application
    OnOffHelper clientHelper ("ns3::TcpSocketFactory", Address());
    clientHelper.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientHelper.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    ApplicationContainer clientApps;
    AddressValue remoteAddress(InetSocketAddress(star.GetHubIpv4Address(2), port));
    clientHelper.SetAttribute("Remote", remoteAddress);
    clientHelper.SetAttribute("PacketSize", UintegerValue(1300));
    clientApps.Add(clientHelper.Install(csmaRightNodes.Get(1))); // n9
    clientApps.Start(Seconds(3.0));
    clientApps.Stop(Seconds(15.0));
}

void configurationOne(PointToPointStarHelper star, NodeContainer csmaRightNodes, NodeContainer csmaLeftNodes, Ipv4InterfaceContainer csmaLeftInterfaces) {
    /* TCP Sink server */
    
    createSink(2300, star.GetHub(), 0, 20.0, false); // n5
    createSink(7457, csmaLeftNodes.Get(0), 0, 20.0, false); // n0

    // OnOff TCP application
    OnOffHelper clientHelper("ns3::TcpSocketFactory", Address());
    clientHelper.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientHelper.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    ApplicationContainer clientApps, clientApps2;
    AddressValue remoteAddress(InetSocketAddress(star.GetHubIpv4Address(2), 2300));
    clientHelper.SetAttribute("Remote", remoteAddress);
    clientHelper.SetAttribute("PacketSize", UintegerValue(2500));
    clientApps.Add(clientHelper.Install(csmaRightNodes.Get(1))); // n9
    clientApps.Start(Seconds(5.0));
    clientApps.Stop(Seconds(15.0));   

    // OnOff TCP application
    OnOffHelper clientHelper2("ns3::TcpSocketFactory", Address());
    clientHelper2.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientHelper2.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    AddressValue remoteAddress2(InetSocketAddress(csmaLeftInterfaces.GetAddress(0), 7457)); // n0
    clientHelper2.SetAttribute("Remote", remoteAddress2);
    clientHelper2.SetAttribute("PacketSize", UintegerValue(5000));
    clientApps2.Add(clientHelper2.Install(csmaRightNodes.Get(0))); // n8
    clientApps2.Start(Seconds(2.0));
    clientApps2.Stop(Seconds(9.0));
}

void createClient(Ptr<Node> node, Ipv4InterfaceContainer csmaLeftInterfaces, double tinit, double tfinish) {
    UdpEchoClientHelper echoClient(csmaLeftInterfaces.GetAddress(2), 63);
    echoClient.SetAttribute("MaxPackets", UintegerValue(5));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(2.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(2560));

    ApplicationContainer clientApps = echoClient.Install(node);

    // 596EF6 Hex Bytes
    uint8_t fill[] = { 0x59, 0x6E, 0xF6 };
    echoClient.SetFill (clientApps.Get(0), fill, sizeof(fill), 2560);

    clientApps.Start(Seconds(tinit));
    clientApps.Stop(Seconds(tfinish)); 
}

void configurationTwo(PointToPointStarHelper star, NodeContainer csmaRightNodes, NodeContainer csmaLeftNodes, Ipv4InterfaceContainer csmaLeftInterfaces, Ipv4InterfaceContainer csmaRightInterfaces) {

    /* Client-Server UDP Echo */

    UdpEchoServerHelper echoServer(63);

    ApplicationContainer serverApps = echoServer.Install(csmaLeftNodes.Get(2));
    serverApps.Start(Seconds(0.0));
    serverApps.Stop(Seconds(20.0));

    createClient(csmaRightNodes.Get(0), csmaLeftInterfaces, 3.0, 13.0); // n8

    /* TCP Sink server */

    createSink(2300, star.GetHub(), 0, 20.0, false);
    createSink(7454, csmaLeftNodes.Get(0), 0, 20.0, true);

    // OnOff TCP application
    OnOffHelper clientHelper("ns3::TcpSocketFactory", Address());
    clientHelper.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientHelper.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    ApplicationContainer clientApps, clientApps2;
    AddressValue remoteAddress(InetSocketAddress(star.GetHubIpv4Address(2), 2300));
    clientHelper.SetAttribute("Remote", remoteAddress);
    clientHelper.SetAttribute("PacketSize", UintegerValue(2500));
    clientApps.Add(clientHelper.Install(csmaRightNodes.Get(1))); // n9
    clientApps.Start(Seconds(3.0));
    clientApps.Stop(Seconds(9.0));   

    // OnOff UDP application
    OnOffHelper clientHelper2("ns3::UdpSocketFactory", Address());
    clientHelper2.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientHelper2.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    AddressValue remoteAddress2(InetSocketAddress(csmaLeftInterfaces.GetAddress(0), 7454)); // n0
    clientHelper2.SetAttribute("Remote", remoteAddress2);
    clientHelper2.SetAttribute("PacketSize", UintegerValue(5000));
    clientApps2.Add(clientHelper2.Install(csmaRightNodes.Get(0))); // n8
    clientApps2.Start(Seconds(5.0));
    clientApps2.Stop(Seconds(15.0)); 
}