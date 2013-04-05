#include "Socket.H"
#include "Packet.H"
#include "RouterTable.H"

queue <SoRData *> pktQ;
UDPSocket *sock = new UDPSocket(routerlistnPort);


void* routeFunction_Recv(void * param)
{
	cout<<"Thread started: routeFunction_recv"<<endl;
	//queue <SoRData> pktQ;
	try {
		cout << "thread start" << endl;
		int recvMsgSize;                  // Size of received message
			string sourceAddress;             // Address of datagram source
			string forwardAddress;              //address of data forwarding
			unsigned short sourcePort;        // Port of datagram source
			PacketCnt *pcnt;
			struct SoRData *data = new SoRData;// = (struct SoRData*)malloc(sizeof(struct SoRData));
			struct SoRData *data1 = new SoRData;// = (struct SoRData*)malloc(sizeof(struct SoRData));
			for (;;) {  // Run forever
				// Block until receive message from a client
					recvMsgSize = sock->recvFrom(data, sizeof(SoRData), sourceAddress, sourcePort);
					memcpy(data1, data, sizeof(SoRData));
//					cout << "Recive---------------------" << endl;
//					cout << "packetID: " << "	"  << "caplen: " << "	"  <<"content: " << "	"  << "sor_flg: " << "	"  << endl;
//					 cout << data->packet_id << "	" << data->pcap_hdr.caplen << "	"<< data->pcap_pkt << "	" << data->sor_flg << endl;

				if(data1->sor_flg == 0){
					int packet_size = data1->pcap_hdr.caplen + sizeof(PacketCnt);
					pcnt = (PacketCnt *)malloc(packet_size);
					memcpy(&(pcnt->pcap_hdr), &(data1->pcap_hdr), sizeof(struct pcap_pkthdr));
					memcpy(pcnt->pcap_pkt, data1->pcap_pkt , pcnt->pcap_hdr.caplen);
/*
					PacketCnt *packet_cnt;			// pointer to libpcap packet structure(w/ header)
					unsigned char* packet;			// ponter to packet binary
					struct ip* ip_header;			// pointer to IP header structure
					struct tcphdr* tcp_header;		// pointer to TCP header structure
					unsigned int l2_header_size;	// L2(MAC) header size
					unsigned int l3_header_size;	// L3(IP) header size
					unsigned int l4_header_size;	// L4(TCP/UDP) header size

					unsigned int packet_size_cap;	// captured packet size(returned by libpcap)
					unsigned int packet_size_org;	// original packet size(returned by libpcap)
					unsigned int content_size;		// payload size
					unsigned int l7_content_size;	// Layer 7 content size(with HTTP/1.1 decode, etc)

					unsigned char* l3_header;		// pointer to L3(MAC) header
					unsigned char* l4_header;		// pointer to L4(TCP/UDP) header
					unsigned char* content;			// pointer to payload begin
					unsigned char* l7_content;		// pointer to Layer7 content begin

					struct in_addr src_ip;			// Source IP
					struct in_addr dst_ip;			// Destination IP
					unsigned int protocol;			// Transport Protocol(ex: TCP:6)
					bool ack;						// TCP ACK flag
					bool fin;						// TCP FIN flag
					bool syn;						// TCP SYN flag
					bool urg;						// TCP URG flag
					bool psh;						// TCP PUSH flag
					bool rst;						// TCP RESET flag
					unsigned int src_port;
					unsigned int dst_port;
					unsigned int seq_no;
*/




//					if(src_port == 80 || dst_port == 80){
					/*
						cout << "before pcap src_ip: " << inet_ntoa(src_ip) << endl;
						cout << "before pcap dst ip: " << inet_ntoa(dst_ip) << endl;
						cout <<"before src_port :" << src_port << endl;
						cout <<"before dst_port :" << dst_port << endl;
*/
						Packet *pkt = new Packet(pcnt);
						/*
						cout << "after pcap src_ip: " <<  inet_ntoa(pkt->GetSrcIP()) << endl;
						cout << "after pcap dst_ip: " <<  inet_ntoa(pkt->GetDstIP()) << endl;
						cout << "after pcap src port: " << pkt->GetSrcPort() << endl;
						cout << "after pcap dst port: " << pkt->GetDstPort() << endl;
						*/

//						cout << "SYN: " << pkt->GetSyn() << endl;
//						cout << "ACK: " << pkt->GetAck() << endl;

						/*
						for(int i = 0; i < 1400; i++){
							cout << pkt->GetL7Content()[i];
						}
						*/
						//cout << "Master start" << endl;
						master->Proc(pkt);
						//sleep(1);
						//cout << "Master end" << endl;
//					}
				}
					pthread_mutex_lock(&mut);

					pktQ.push(data1);
				//	cout << "Success queue" << endl;
					pthread_mutex_unlock(&mut);
				}
		}
	catch (SocketException &e) {
		  cerr << e.what() << endl;
		  exit(1);
		}
	return 0;
}



void* routeFunction_Send(void * param)
{
	cout<<"Thread started: routeFunction_send"<<endl;
	struct SoRData *data ;//= (struct SoRData*)malloc(sizeof(struct SoRData));
	struct SoRData *fdata = new SoRData;
	try {

	string sourceAddress;             // Address of datagram source
	string forwardAddress;              //address of data forwarding
	unsigned short sourcePort;        // Port of datagram source
	string tmp_str;
	SoRSimHeader *sor_hdr;

	for (;;) {  // Run forever
		if(!pktQ.empty())
		{
			pthread_mutex_lock(&mut);
			fdata = pktQ.front();

//			cout << "Send---------------------" << endl;
//			cout << "packetID: " << data->packet_id  << endl;
//			cout << "from: " << "	" << "nexthop: " << "	"  << "port: "  << endl;

//			cout << fdata->src_ip << "	" << router_table->GetMapNextHop(fdata->src_ip) << "	" << router_table->GetMapPort(fdata->src_ip) << endl;
/*
			if(router_table->GetUpdateFlg() != 0){
				router_table->UpdateRoutingTable();
			}
			*/
			if(router_table->GetMapPort(fdata->src_ip) != 0){
				sock->sendTo(fdata, sizeof(SoRData), router_table->GetMapNextHop(fdata->src_ip), router_table->GetMapPort(fdata->src_ip));
			}else{
				cout << "Not found in routing table" << endl;
			}
			pktQ.pop();
			pthread_mutex_unlock(&mut);
//			cout << "Data has sent" << endl;
		}
	 }

	} catch (SocketException &e) {
			cerr << e.what() << endl;
			exit(1);
  }
  return 0;
}
