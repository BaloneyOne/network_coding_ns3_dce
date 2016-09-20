/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "nicholson-nc-udp-router-agent.h"
#include "tcp.h"
#include "ip.h"
#include "udp.h"
#include "agent.h"
#include "app.h"
#include "random.h"


static class UdpRouterAgentClass:public TclClass
{
public:
  UdpRouterAgentClass ():TclClass ("Agent/UDPRouter")
  {
  }
  TclObject *create (int, const char *const *)
  {
    return (new UdpRouterAgent ());
  }
}

class_udp_router_agent;

UdpRouterAgent::UdpRouterAgent(): Agent(PT_netcod) {
;
}


void UdpRouterAgent::recv(Packet* p , Handler*) {
  hdr_ip *iph	= hdr_ip::access(p);
  iph->daddr() = 4;
  send(p, 0);
}





namespace ns3 {

/* ... */


}

