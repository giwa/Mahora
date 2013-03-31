/*
 *   C++ sockets on Unix and Windows
 *   Copyright (C) 2002
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <string.h>
#include <string>
#include <iostream>
#include "PracticalSocket.h"      // For UDPSocket and SocketException
#include <cstdlib>



const int ECHOMAX = 1800;     // Longest string to echo

int main(int argc, char *argv[]) {

  if (argc != 2) {                  // Test for correct number of parameters
    cerr << "Usage: " << argv[0] << " <Server Port>" << endl;
    exit(1);
  }

  unsigned short echoServPort = atoi(argv[1]);     // First arg:  local port

  try {
    UDPSocket sock(echoServPort);

    char echoBuffer[ECHOMAX];         // Buffer for echo string
    int recvMsgSize;                  // Size of received message
    string sourceAddress;             // Address of datagram source
    unsigned short sourcePort;        // Port of datagram source
    for (;;) {  // Run forever
      // Block until receive message from a client
      recvMsgSize = sock.recvFrom(echoBuffer, ECHOMAX, sourceAddress,
                                      sourcePort);

      cout << "Received packet from " << sourceAddress << ":"
           << sourcePort << endl;
		   echoBuffer[recvMsgSize] = '\0';
		   cout << echoBuffer << endl;
		   cout << recvMsgSize << endl;

	unsigned char* packet;			// ponter to packet binary
	unsigned char* l3_header;
	struct ip* ip_header;
	struct in_addr src_ip;
	struct in_addr dst_ip;
	struct timeval timestamp;

	PacketCnt *pcnt;
	//SoRData *data;
	int packet_size = recvMsgSize - sizeof(SoRSimHeader);
	cout << packet_size <<  endl;
	cout << "echoBuffer: "<< recvMsgSize << endl;
	cout << "pkthdr" << sizeof(struct pcap_pkthdr) <<  endl;
	cout << "SoRSim" << sizeof(struct SoRSimHeader) <<  endl;
	pcnt = (PacketCnt *)malloc(packet_size);
	memcpy(&(pcnt->pcap_hdr), echoBuffer + sizeof(SoRSimHeader), sizeof(struct pcap_pkthdr));
	cout << "caplen " << pcnt->pcap_hdr.caplen << endl;
	memcpy(pcnt->pcap_pkt, echoBuffer + sizeof(SoRSimHeader) + sizeof(struct pcap_pkthdr), pcnt->pcap_hdr.caplen);

	cout << "NEGI pass" << endl;
//	timestamp = packet_cnt->pcap_hdr.ts;
	packet = (unsigned char *)malloc(sizeof(PacketCnt) + pcnt->pcap_hdr.caplen);
	memcpy(packet, pcnt, sizeof(PacketCnt) + pcnt->pcap_hdr.caplen);

	l3_header = packet + sizeof(struct ether_header); //IP header
	ip_header = (struct ip *)l3_header;
	src_ip = ip_header->ip_src;
	dst_ip = ip_header->ip_dst;


	cout << "src_ip: " << inet_ntoa(src_ip)  << endl;
	cout << "content: "<<  pcnt->pcap_pkt << endl;

      sock.sendTo(echoBuffer, recvMsgSize, sourceAddress, sourcePort);
    }
  } catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  // NOT REACHED

  return 0;
  }
