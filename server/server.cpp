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
#include "../socket/PracticalSocket.H"      // For UDPSocket and SocketException
#include <cstdlib>

using namespace std;


int main(int argc, char *argv[]) {

  if (argc != 2) {                  // Test for correct number of parameters
    cerr << "Usage: " << argv[0] << " <Server Port>" << endl;
    exit(1);
  }

  unsigned short echoServPort = atoi(argv[1]);     // First arg:  local port

  try {
    UDPSocket sock(echoServPort);

	size_t cnt = 0;
    int recvMsgSize;                  // Size of received message
    string sourceAddress;             // Address of datagram source
    unsigned short sourcePort;        // Port of datagram source
	SoRData *data = new SoRData;
    for (;;) {  // Run forever
      // Block until receive message from a client
      recvMsgSize = sock.recvFrom(data, sizeof(SoRData), sourceAddress, sourcePort);
//	  cout << "src IP " << data->src_ip << endl;
	  string server_ip = "192.168.1.2";
	  memcpy(data->src_ip, server_ip.c_str() , server_ip.size());
	  data->src_ip[server_ip.size()] = '\0';
	  data->sor_flg = 1;



		cout << "PacketID: " << data->packet_id << endl;
//		cout << "IP: " << sourceAddress << "	" << "Port: " << sourcePort << endl;
      sock.sendTo(data, sizeof(SoRData), sourceAddress, sourcePort);
//	  cout << "send" << endl;
    }
  } catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  // NOT REACHED

  return 0;
  }
