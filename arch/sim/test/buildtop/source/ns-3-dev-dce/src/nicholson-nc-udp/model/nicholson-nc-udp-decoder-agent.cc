/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2015
 *	Nicholson Eugene
 *	All rights reserved.

 * @author 	Nicholson Eugene
 */
 

#include "nicholson-nc-udp-decoder-agent.h"
#include "tcp.h"
#include "ip.h"
#include "udp.h"
#include "agent.h"
#include "app.h"
#include "random.h"
#include "/home/nick/network-sim/network_coding_ns3_dce-master/arch/sim/test/buildtop/source/ns-3-dev-dce/src/nicholson-nc-udp/model/network_coding/Galois_Field.h"
#include "/home/nick/network-sim/network_coding_ns3_dce-master/arch/sim/test/buildtop/source/ns-3-dev-dce/src/nicholson-nc-udp/model/network_coding/Matrix.h"
#include "/home/nick/network-sim/network_coding_ns3_dce-master/arch/sim/test/buildtop/source/ns-3-dev-dce/src/nicholson-nc-udp/model/network_coding/Galois_Field.cc"
#include "/home/nick/network-sim/network_coding_ns3_dce-master/arch/sim/test/buildtop/source/ns-3-dev-dce/src/nicholson-nc-udp/model/network_coding/Matrix.cc"

static class UdpDecodeAgentClass:public TclClass
{
public:
  UdpDecodeAgentClass ():TclClass ("Agent/UDPDecoder")
  {
  }
  TclObject *create (int, const char *const *)
  {
    return (new UdpDecodeAgent ());
  }
}

class_udp_decodeagent;

UdpDecodeAgent::UdpDecodeAgent(): Agent(PT_netcod), success(0), blksize(5), field(256) {
  q_ = new PacketQueue;
  vector_of_matrix = new std::vector<Matrix*>();
  vector_of_nc_data_decoder = new std::vector<struct NcData*> ();
}

 
void UdpDecodeAgent::recv(Packet* p , Handler*) {
  hdr_ip *iph	= hdr_ip::access(p);
  hdr_tcp *tph = hdr_tcp::access(p);
  int blkno = tph->blkno();
  int seed  = tph->seed();
  srand(seed);
  std::vector<uint8_t> *coeffs;
  std::vector<uint8_t> *res;
  NcData *d = p->get_nc_data();
  std::cout << "Receiver:: Blkno = " << blkno << " seed = " << seed << std::endl;
  
  if (blkno >= 0) {
	coeffs = new std::vector<uint8_t>(blksize);
	res    = new std::vector<uint8_t>(blksize);
    for (int c = 0; c < blksize; c++) {
      coeffs->at(c) = (uint8_t)(rand() % field);
    }  
    
        
    if ((int)vector_of_nc_data_decoder->size() <= blkno) {
      vector_of_nc_data_decoder->push_back(new struct NcData);
    } 
    vector_of_nc_data_decoder->at(blkno) = d;
        
    GaloisField gf(field);
    if ((int)vector_of_matrix->size() <= blkno) {
      vector_of_matrix->push_back(new Matrix());
      if (blkno >= 1) {        
        vector_of_matrix->at(blkno-1)->printMatrix();
        bool results;
        clock_t startTime = clock();
        results = vector_of_matrix->at(blkno-1)->gaussianElimination(false);  
        double local_time2 = Scheduler::instance().clock();
        cout << "Decoding time" << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds. of blk " << blkno-1 << endl;
        if (results == 1) {
		  success ++;
		  for (int i = 0; i < vector_of_nc_data_decoder->at(blkno-1)->payloads_->size(); i++) {
		    Packet *pkt;
		    pkt = vector_of_nc_data_decoder->at(blkno-1)->payloads_->at(i);
		    hdr_ip *iphpkt	= hdr_ip::access(pkt);
		    iphpkt->daddr() = 3;
		    /* send(pkt,0); */
		  }
		  vector_of_matrix->at(blkno-1)->clearMatrix();
		  vector_of_nc_data_decoder->at(blkno-1)->payloads_->clear(); 
		}
        std::cout << "BLOCK =  " << blkno-1 << " " << "RESULTS =  " << results << " "  
                  << "P_SUCCESS = " << (double)success/(blkno+1) << " " 
                  << "P_FAIL = "    << 1-(double)success/(blkno+1) << std::endl;
      }
    }
    vector_of_matrix->at(blkno)->setGFPtr(gf);
        
    for (u_int j = 0; j < blksize; j++) {
	  res->push_back((uint8_t)(rand() % field));
    }
    vector_of_matrix->at(blkno)->addConstant(vector_of_matrix->at(blkno)->calcConstant(*coeffs, *res));
    vector_of_matrix->at(blkno)->appendRow(*coeffs);
    coeffs->clear();   

  }  
}





namespace ns3 {

/* ... */


}

