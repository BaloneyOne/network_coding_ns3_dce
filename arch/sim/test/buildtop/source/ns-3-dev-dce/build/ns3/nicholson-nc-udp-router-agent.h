/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef NICHOLSON_NC_UDP_H
#define NICHOLSON_NC_UDP_H
#ifndef ns_udp_router_agent_h
#define ns_udp_router_agent_h

#include "timer-handler.h"
#include "packet.h"
#include "agent.h"
#include "tcp.h"

class UdpRouterAgent;

class UdpRouterAgent : public Agent {
public: 
  UdpRouterAgent();
  void recv(Packet*, Handler*);
};



#endif

namespace ns3 {

/* ... */

}

#endif /* NICHOLSON_NC_UDP_H */

