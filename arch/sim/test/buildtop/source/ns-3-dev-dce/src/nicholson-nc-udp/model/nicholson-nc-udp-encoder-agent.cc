/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2015
 *	Nicholson Eugene
 *	All rights reserved.

 * @author 	Nicholson Eugene
 */
 
#include "nicholson-nc-udp-encoder-agent.h"
#include "tcp.h"
#include "ip.h"
#include "udp.h"
#include "agent.h"
#include "app.h"
#include "random.h"
#include "rtp.h"

static class UdpEncodeAgentClass:public TclClass
{
public:
  UdpEncodeAgentClass ():TclClass ("Agent/UDPEncoder")
  {
  }
  TclObject *create (int, const char *const *)
  {
    return (new UdpEncodeAgent ());
  }
}
  
class_udp_encodeagent;

UdpEncodeAgent::UdpEncodeAgent(): Agent(PT_netcod), send_timer_(this), blksize(5), field (256) 
{
  q1_ = new PacketQueue;
  q2_ = new PacketQueue;
  send_timer_.resched((double)(0.1));
  vector_of_nc_data = new std::vector<struct NcData*> ();
  srand(time(NULL));
}

void SendTimer::expire(Event*) {
	a_->send_q2();
}

void UdpEncodeAgent::send_q2() {
  if (q2_->length()) {
	  
  	Packet *p;		
    p = q2_->deque();        
		
    if (p) {
	  hdr_ip *iph	= hdr_ip::access(p);
      iph->daddr() = 2;  
      send(p,0);	  	  		
	}
	
	send_timer_.resched((double)(8. * 1000 / 10000000));	  
  }  
  
  else {
    send_timer_.resched((double)(8. * 1000 / 10000000));
  }    	
}

void UdpEncodeAgent::recv(Packet* p , Handler*) {
  q1_->enque(p);
  if (q1_->length() >= blksize) {	
    generate_nc_data();
  }
}


void UdpEncodeAgent::generate_nc_data() {
  int blk;
  for (int i =0; i < blksize; i++) {		
    Packet *p;
    p = q1_->deque();   
    
    if (p) {			
      int seqno = hdr_rtp::access(p)->seqno();
      int blkno = seqno/blksize;
      std::cout << "Seqno " << seqno << " Blkno " << blkno << std::endl;
      blk = blkno;
      if ((int)vector_of_nc_data->size() <= blkno) {
        vector_of_nc_data->push_back(new struct NcData);
      }          
      vector_of_nc_data->at(blkno)->payloads_->push_back(p);
    }    
  }	
 
  int r = 1;
  for (int i= 0; i < blksize + r; i++) {
	Packet *LC = allocpkt();
    LC->set_nc_data(vector_of_nc_data->at(blk));   
    vector_of_nc_data->at(blk)->payloads_->clear();
    hdr_tcp *tph = hdr_tcp::access(LC);
    tph->blkno() = blk;
    tph->seed() = (uint32_t) rand()%(256);
	q2_->enque(LC);
  }
  
}



namespace ns3 {

/* ... */


}

