/*By default, the 
simulation runs for 60 simulated seconds. 
The number of nodes are  varying between 25 to 250.
* Nodes move according to RandomWaypointMobilityModel with a speed of varying between 
* 15 m/s to 20 m/s, 
*transmission range of vechicles is 100 m and 
*no pause time within a 1000x1000 m^2 region.  
* The WiFi is in ad hoc mode with a 11 Mb/s rate (802.11b) and a RangePropagationLossModel.
* The transmit power is set to 7.5 dBm. 
*size of packet is set to 64 bytes. 
*transmission rate is set 4 kbps. 
wifi standard used is 802.11b. routing protocol used is AODV. 
* It is possible to change the mobility and density of the network by
* directly modifying the speed and the number of nodes.  It is also
* possible to change the characteristics of the network by changing
* the transmit power (as power increases, the impact of mobility
* decreases and the effective density increases). */
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <ctime>
#include <map>
#include "ns3/ipv4.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/aodv-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/point-to-point-module.h"


using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("vanet");

class RoutingExperiment
{
public:
  RoutingExperiment ();
  void Run (int nSinks, double txp, string CSVfileName);
  string CommandSetup (int argc, char **argv);

private:
  Ptr<Socket> SetupPacketReceive (Ipv4Address receiver, Ptr<Node> node);
  void ReceivePacket (Ptr<Socket> socket);  

  uint32_t i;
  uint32_t start_s;
  uint32_t stop_s;  
  uint32_t port;
  uint32_t bytesTotal;
  uint32_t packetsReceived;
  uint32_t direction;
  string m_CSVfileName; 
  string m_protocolName; 
  string s;
  bool m_traceMobility;
  uint32_t m_protocol;
};

RoutingExperiment::RoutingExperiment ()
  : 
	i(0),
	start_s(0),
	stop_s(0),
	port (9),
    bytesTotal (0),
    packetsReceived (0),
	direction(0),	
    m_CSVfileName ("vanet.output.csv"),
	s(""),
    m_traceMobility (false),
    m_protocol (2) // AODV
{
}

void RoutingExperiment::ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  Address senderAddress;     
  ofstream myfile;   
  if (packet = socket->RecvFrom (senderAddress))
  { 	  
	stop_s=clock();
	cout << "Total Execution Time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC) << endl;	    
	InetSocketAddress addr = InetSocketAddress::ConvertFrom (senderAddress);
	cout << "received one packet from " << addr.GetIpv4 ();
	uint8_t *buffer = new uint8_t[packet->GetSize ()];
	packet->CopyData(buffer, packet->GetSize ());
	s = string(buffer, buffer+packet->GetSize());
	cout<<" content is :"<<s<<"\n";		 	
  }  
}

Ptr<Socket>
RoutingExperiment::SetupPacketReceive (Ipv4Address receiver, Ptr<Node> node)
{
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> sink = Socket::CreateSocket (node, tid);  
  InetSocketAddress local = InetSocketAddress (receiver, port);
  sink->Bind (local);    
  sink->SetRecvCallback (MakeCallback (&RoutingExperiment::ReceivePacket, this));  
  return sink;
}

string RoutingExperiment::CommandSetup (int argc, char **argv)
{
  CommandLine cmd;
  cmd.AddValue ("CSVfileName", "The name of the CSV output file name", m_CSVfileName);
  cmd.AddValue ("traceMobility", "Enable mobility tracing", m_traceMobility);
  cmd.AddValue ("protocol", "1=OLSR;2=AODV;3=DSDV;4=DSR", m_protocol);
  cmd.Parse (argc, argv);
  return m_CSVfileName;
}

int main (int argc, char *argv[])
{
  RoutingExperiment experiment;
  string CSVfileName = experiment.CommandSetup (argc,argv);

  //blank out the last output file and write the column headers
  ofstream out (CSVfileName.c_str ());
  out << "SimulationSecond," <<
  "ReceiveRate," <<
  "PacketsReceived," <<
  "NumberOfSinks," <<
  "RoutingProtocol," <<
  "TransmissionPower" <<
  endl;
  out.close ();

  int nSinks = 10;
  double txp = 7.5;
  experiment.Run (nSinks, txp, CSVfileName);
}

void RoutingExperiment::Run (int nSinks, double txp, string CSVfileName)
{
  Packet::EnablePrinting (); 

  int nWifis = 25;

  double TotalTime = 60.0;
  string rate ("4096bps");
  string phyMode ("DsssRate11Mbps");
  string tr_name ("vanet");  

  m_protocolName = "protocol";

  Config::SetDefault  ("ns3::OnOffApplication::PacketSize",StringValue ("64"));
  Config::SetDefault ("ns3::OnOffApplication::DataRate",  StringValue (rate));
  //Set Non-unicastMode rate to unicast mode
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode",StringValue (phyMode));

  NodeContainer adhocNodes;
  adhocNodes.Create (nWifis);

  // setting up wifi phy and channel using helpers
  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                  "DataMode",StringValue (phyMode),
                                  "ControlMode",StringValue (phyMode));

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);
  wifiPhy.Set ("TxPowerStart",DoubleValue (txp));
  wifiPhy.Set ("TxPowerEnd", DoubleValue (txp));

  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss("ns3::RangePropagationLossModel", "MaxRange",DoubleValue(100.0));

  wifiPhy.SetChannel (wifiChannel.Create ());
  
  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::AdhocWifiMac");
  NetDeviceContainer adhocDevices = wifi.Install (wifiPhy, wifiMac, adhocNodes);

  AodvHelper aodv;
  Ipv4ListRoutingHelper list;
  list.Add (aodv, 100);  

  InternetStackHelper internet;
  internet.SetRoutingHelper (list);
  internet.Install (adhocNodes);

  NS_LOG_INFO ("assigning ip address");

  Ipv4AddressHelper addressAdhoc;
  addressAdhoc.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer adhocInterfaces;
  adhocInterfaces = addressAdhoc.Assign (adhocDevices);

  OnOffHelper onoff1 ("ns3::UdpSocketFactory",Address ());
  onoff1.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"));
  onoff1.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"));     
  
  for (int i = 0; i <2 ; i++)
    {
      Ptr<Socket> sink = SetupPacketReceive (adhocInterfaces.GetAddress (i), adhocNodes.Get (i));
      //adhocInterfaces.GetAddress (i): Receiver,adhocNodes.Get (i) :Sender      
    } 
  MobilityHelper mobilityAdhoc;  
  
  ObjectFactory pos;
  pos.SetTypeId ("ns3::RandomBoxPositionAllocator");
  pos.Set ("X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=1000.0]"));
  pos.Set ("Y", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=1000.0]"));
  // we need antenna height uniform [1.0 .. 2.0] for loss model
  pos.Set ("Z", StringValue ("ns3::UniformRandomVariable[Min=1.0|Max=2.0]"));

  Ptr<ListPositionAllocator> positionAlloc = CreateObject <ListPositionAllocator>();
    positionAlloc ->Add(Vector(10, 10,10)); // node0
    positionAlloc ->Add(Vector(50, 10,10)); // node1
    positionAlloc ->Add(Vector(90, 10,10)); // node2
    positionAlloc ->Add(Vector(10, 30,10)); // node3
    positionAlloc ->Add(Vector(50, 30,10)); // node4
    positionAlloc ->Add(Vector(90, 30,10)); // node5
    positionAlloc ->Add(Vector(10, 50,10)); // node6
    positionAlloc ->Add(Vector(50, 50,10)); // node7
    positionAlloc ->Add(Vector(90, 50,10)); // node8
    positionAlloc ->Add(Vector(10, 70,10)); // node9
    positionAlloc ->Add(Vector(50, 70,10)); // node10
    positionAlloc ->Add(Vector(90, 70,10)); // node11
    positionAlloc ->Add(Vector(10, 90,10)); // node12
    positionAlloc ->Add(Vector(50, 90,10)); // node13
    positionAlloc ->Add(Vector(90, 90,10)); // node14
    positionAlloc ->Add(Vector(10, 110,10)); // node15
    positionAlloc ->Add(Vector(50, 110,10)); // node16
    positionAlloc ->Add(Vector(90, 110,10)); // node17
    positionAlloc ->Add(Vector(10, 140,10)); // node18
    positionAlloc ->Add(Vector(50, 140,10)); // node19

  mobilityAdhoc.SetPositionAllocator(positionAlloc);

  double speed=17.0;
  mobilityAdhoc.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobilityAdhoc.InstallAll();
  for (uint i=0 ; i < adhocNodes.GetN() ; i++)
  {
  	Ptr<ConstantVelocityMobilityModel> mob = adhocNodes.Get(i)->GetObject<ConstantVelocityMobilityModel>();
  	mob->SetVelocity(Vector(speed, 0, 0));
  }  
  ///////////////////////////////////////////////////////////////
  //                                                            /
  //   			CONSTRUCTION OF LAN                             /
  //                                                            /
  ///////////////////////////////////////////////////////////////  
  NodeContainer LanNodes;
  LanNodes.Create (1);
  
  NodeContainer lan (adhocNodes.Get (1), LanNodes);  
  
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  
  NetDeviceContainer p2pDevices = p2p.Install(lan);  
  
  internet.Install (LanNodes);
  
  Ipv4AddressHelper ipAddrs;
  ipAddrs.SetBase ("172.16.1.0", "255.255.255.0");
  //ipAddrs.Assign (lanDevices);  
  
  Ipv4InterfaceContainer lanInterfaces;
  lanInterfaces = ipAddrs.Assign (p2pDevices);
  
  MobilityHelper mobilityLan;
  Ptr<ListPositionAllocator> subnetAlloc =CreateObject<ListPositionAllocator> ();
      
  subnetAlloc->Add (Vector (15.0, 110, 0.0));  
        
  mobilityLan.PushReferenceMobilityModel (adhocNodes.Get (1));
  mobilityLan.SetPositionAllocator (subnetAlloc);
  mobilityLan.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobilityLan.Install (LanNodes);
    
  map<string,string> map1;
  string key, value;
  ifstream openfile;
  openfile.open("output.csv",ios::in); 
  while(true)
  {    
     if (!getline(openfile, key, ',')) break;     
     getline(openfile, value, '\n');
     map1[key]=value;     
  }
  openfile.close(); 
  
  NS_LOG_INFO ("Run Simulation.");  
  TypeId tidd = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> source = Socket::CreateSocket (adhocNodes.Get (0), tidd); // Sender
  Ptr<Socket> source1 = Socket::CreateSocket (adhocNodes.Get (1), tidd); // Sender=WIFI Node
  Ptr<Socket> source2 = Socket::CreateSocket (LanNodes.Get (0), tidd); // Sender=LAN Node  
  
  Ptr<Socket> sink = SetupPacketReceive (adhocInterfaces.GetAddress (1), adhocNodes.Get (0));
  //Send from wireless to lan
  Ptr<Socket> sink1 = SetupPacketReceive (lanInterfaces.GetAddress (0),adhocNodes.Get (1));
  //Send from lan to wireless
  Ptr<Socket> sink2 = SetupPacketReceive (adhocInterfaces.GetAddress (1), LanNodes.Get (0));
  Ptr<Socket> sink3 = SetupPacketReceive (adhocInterfaces.GetAddress (0), adhocNodes.Get (1));				  		
       
  
  //RSU Setup

  /*string idrsu="jhalwa";  
  uint32_t yj=456976;
  uint32_t ej=6859;   
  ostringstream msg1;	// Send IDRSU from RSU to RTA 
  msg1<<idrsu<<yj<<ej;
  Ptr<Packet> pkt1 = Create<Packet> ((uint8_t*) msg1.str().c_str(), msg1.str().length()); 
  start_s=clock();     
  source1->SendTo(pkt1,0,InetSocketAddress(Ipv4Address("172.16.1.1"), port));  */ 
    
  /*string idrsu="jhalwa";  
  uint32_t yj=456976;
  uint32_t ej=6859;  
  string signature="d3e48573fbee9bced80e3f735f826caf039d6ba9";  
  ostringstream msg2;  // Send Certifiacte from RTA to RSU
  msg2<<idrsu<<yj<<ej<<signature;   
  Ptr<Packet> pkt3 = Create<Packet> ((uint8_t*) msg2.str().c_str(), msg2.str().length()); 
  start_s=clock();      
  source2->SendTo(pkt3,0,InetSocketAddress(Ipv4Address("10.1.1.2"), port)); */

  //IPIDVI Computation    

  /*ostringstream msg; //Send vehicles encrypted identity from vehicle to RSU    
  msg<<map1["eid_vi"];
  Ptr<Packet> pkt = Create<Packet> ((uint8_t*) msg.str().c_str(), msg.str().length());
  start_s=clock();
  source->SendTo(pkt,0,InetSocketAddress(Ipv4Address("10.1.1.2"), port)); */
    		  
  /*ostringstream msg1; //Send vehicles encrypted identity from RSU to RTA
  msg1<<map1["eid_vi"];  
  Ptr<Packet> pkt1 = Create<Packet> ((uint8_t*) msg1.str().c_str(), msg1.str().length()); 
  start_s=clock();     
  source1->SendTo(pkt1,0,InetSocketAddress(Ipv4Address("172.16.1.1"), port)); */
    	 
  /*ostringstream msg2;  // Send IPID_VI from RTA to RSU
  msg2<<map1["IPID_VI"];   
  Ptr<Packet> pkt3 = Create<Packet> ((uint8_t*) msg2.str().c_str(), msg2.str().length()); 
  start_s=clock();      
  source2->SendTo(pkt3,0,InetSocketAddress(Ipv4Address("10.1.1.2"), port)); */
      
  /*ostringstream msg4; //Send IPID_VI from RSU to V
  msg4<<map1["IPID_VI"];  
  Ptr<Packet> pkt2 = Create<Packet> ((uint8_t*) msg4.str().c_str(), msg4.str().length());
  start_s=clock();
  source1->SendTo(pkt2,0,InetSocketAddress(Ipv4Address("10.1.1.1"), port)); */  
    
  /*ostringstream msg4; //RSU broadcasts its signature
  msg4<<map1["signature"];  
  Ptr<Packet> pkt2 = Create<Packet> ((uint8_t*) msg4.str().c_str(), msg4.str().length());
  start_s=clock();
  source1->SendTo(pkt2,0,InetSocketAddress(Ipv4Address("10.1.1.1"), port)); */
        
  //Member Secrets Generation
  /*uint32_t f=361;
  string id_rsu="jhalwa";
  string rho_j="wRbnMHXdQoIh+TCeyXO4BtUTWBV0hVIlWHdcGyeoQCU=";
  string Toht="60min";    
  ostringstream msg; //Send Computed data from vehicle to RSU    
  msg<<f<<id_rsu<<rho_j<<Toht;
  Ptr<Packet> pkt = Create<Packet> ((uint8_t*) msg.str().c_str(), msg.str().length());
  start_s=clock();
  source->SendTo(pkt,0,InetSocketAddress(Ipv4Address("10.1.1.2"), port)); */
    
  /*
  uint32_t f=361;
  string id_rsu="jhalwa";
  string rho_j="wRbnMHXdQoIh+TCeyXO4BtUTWBV0hVIlWHdcGyeoQCU=";
  string Toht="60min";
  ostringstream msg1; //Send Computed data from RSU to RTA
  msg1<<f<<id_rsu<<rho_j<<Toht;  
  Ptr<Packet> pkt1 = Create<Packet> ((uint8_t*) msg1.str().c_str(), msg1.str().length());
  start_s=clock();      
  source1->SendTo(pkt1,0,InetSocketAddress(Ipv4Address("172.16.1.1"), port)); */
    
  /*ostringstream msg2;  // RTA sends 1 to RSU if vehicle is valid
  msg2<<1;   
  Ptr<Packet> pkt3 = Create<Packet> ((uint8_t*) msg2.str().c_str(), msg2.str().length()); 
  start_s=clock();      
  source2->SendTo(pkt3,0,InetSocketAddress(Ipv4Address("10.1.1.2"), port));   */ 
    
  // RSU transmists MS to V   
  /*string rho_j1="0mWatdyTv/5RZpG+Vs2d5BUjgk6zdmc09C1pcRel73Po6m4CYmsLjl7AgN811pgqG6sk+sRVniuJORTctDEzI6DfK7upW/1DaOOd2iT5wUo=";
  string H4f="b6e7a22c25e9e7146913841b0995288c688f9bf8";
  ostringstream msg4; //RSU broadcasts rhoj and h4f
  msg4<<rho_j1<<H4f;  
  Ptr<Packet> pkt2 = Create<Packet> ((uint8_t*) msg4.str().c_str(), msg4.str().length());
  start_s=clock();
  source1->SendTo(pkt2,0,InetSocketAddress(Ipv4Address("10.1.1.1"), port));  */
    

  /*string ppidvi="084347e5a52655681300bdb191795e2bb14dcba9";
  string emivi="qPSoPbc+IK3Zt74Xk9+A9CCG99rPrS2Js3gvPx6SqnM=";
  string msgsig="dd9447cf0193e22ebcc104087f6c56a7e0a11672";
  ostringstream msg; //Send PPID, message and signature from vehicle to RSU  
  msg<<ppidvi<<emivi<<msgsig;
  Ptr<Packet> pkt = Create<Packet> ((uint8_t*) msg.str().c_str(), msg.str().length());
  start_s=clock();
  source->SendTo(pkt,0,InetSocketAddress(Ipv4Address("10.1.1.2"), port));   */
    
  Simulator::Stop (Seconds (TotalTime));
  Simulator::Run (); 
  
  Simulator::Destroy ();

}

