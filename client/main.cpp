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
#include <pcap.h>
using namespace std;

string serv_address;
char* port_num;
size_t cnt = 0;


UDPSocket *sock = new UDPSocket(52000);

/*
 * workhorse function, we will be modifying this function
 */
void pcap_callback(u_char *userdata, const struct pcap_pkthdr *h, const u_char *p) {

	unsigned char* packet;			// ponter to packet binary
	unsigned char* l3_header;
	struct ip* ip_header;
	struct in_addr src_ip;
	struct in_addr dst_ip;
	struct timeval timestamp;

	cnt++;
	PacketCnt *pcnt;
	int packet_size = sizeof(struct pcap_pkthdr) + h->caplen;

	pcnt = (PacketCnt *)malloc(packet_size);
	memcpy(&(pcnt->pcap_hdr), h, sizeof(struct pcap_pkthdr));
	memcpy(pcnt->pcap_pkt, p , h->caplen);


	SoRData *data = new SoRData;
	int data_size = sizeof(struct pcap_pkthdr) + h->caplen;
	data->packet_id = cnt;
	memcpy(&(data->pcap_hdr), h, sizeof(struct pcap_pkthdr));
	memcpy(data->pcap_pkt, p, h->caplen);
	data->pkt_len = data_size;
	cout << "Packet ID: " << cnt << endl;

	timestamp = data->pcap_hdr.ts;
	packet = (unsigned char *)malloc(h->caplen);
	memcpy(packet, data->pcap_pkt, h->caplen);


//delete data;

//	cout << "caplen: " << data->pcap_hdr.caplen  << endl;
//	cout << "src_ip: " << inet_ntoa(src_ip)  << endl;
//	cout << "content: "<<  data->pcap_pkt << endl;

	unsigned char* l4_header;		// pointer to L4(TCP/UDP) header
	unsigned char* content;			// pointer to payload begin
	unsigned char* l7_content;		// pointer to Layer7 content begin

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
	struct tcphdr* tcp_header;		// pointer to TCP header structure

	//packet = (unsigned char *)malloc(pcnt->pcap_hdr.caplen);
	memcpy(packet, pcnt->pcap_pkt, pcnt->pcap_hdr.caplen);

	l3_header = packet + sizeof(struct ether_header); //IP header
	ip_header = (struct ip *)l3_header;
	src_ip = ip_header->ip_src;
	dst_ip = ip_header->ip_dst;
	protocol = ip_header->ip_p;

	l4_header = l3_header + ip_header->ip_hl*4; //TCP/UDP header

	if(protocol == IPPROTO_TCP){
		//PACKET_DEBUG(RED cout << "TCP Packet!" << endl ;RESET);
		tcp_header = (struct tcphdr *)l4_header;
		src_port = ntohs(tcp_header->source);
		dst_port = ntohs(tcp_header->dest);
		seq_no = ntohl(tcp_header->seq);
		ack = tcp_header->ack;
		fin = tcp_header->fin;
		syn = tcp_header->syn;
		urg = tcp_header->urg;
		psh = tcp_header->psh;
		rst = tcp_header->rst;
	cout << "SYN: " << syn << endl;
	cout << "ACK: " << ack << endl;
	if(syn && !ack){
		cout << "STREAM START-----------------" << endl;
		if(cnt == 785){
			l3_header = packet  + sizeof(struct ether_header); //IP header
			ip_header = (struct ip *)l3_header;
			src_ip = ip_header->ip_src;
			dst_ip = ip_header->ip_dst;
			cout << "packet id: " << data->packet_id << endl;
			cout << "caplen: " << data->pcap_hdr.caplen  << endl;
			cout << "src_ip: " << inet_ntoa(src_ip) << endl;
			cout << "dst_ip: " << inet_ntoa(dst_ip) << endl;
			cout << "src_port" << src_port << endl;
			cout << "dst_port" << dst_port << endl;
			for(int i = 0; i < 1500; i++){
				printf("%c", pcnt->pcap_pkt[i]);
			}
			cout << "data-----------------" << endl;
			cout << "data-----------------" << endl;
			for(int i = 0; i < 1500; i++){
				printf("%c", pcnt->pcap_pkt[i]);
			}
		}
		sleep(1);
	}
	}



unsigned short echoServPort = Socket::resolveService(port_num, "udp");


try {
memcpy(data->src_ip,"192.168.1.6",15);
data->src_port = sock->getLocalPort();
data->sor_flg = 0;


  sockaddr_in addr;
unsigned int addr_len = sizeof(addr);

cout << "src_ip sim: "<<  data->src_ip << endl;
// Send the string to the server
sock->sendTo(data, data_size, serv_address, echoServPort);
cout << "Data have sent-------------------- " << endl;
// Receive a response
int respStringLen;                  // Length of received response
respStringLen = sock->recv(data, sizeof(SoRData));
cout << "Data have recieved-------------------- " << endl;
delete data;
//	free(packet);

// Destructor closes the socket

} catch (SocketException &e) {
cerr << e.what() << endl;
exit(1);
}

}


int main(int argc,char **argv)
{
	if(argc < 4 || argc > 4){
		cerr << "Usage: " << argv[0]
			<< "<Server> <Server Port> <Path to pcapfile>" << endl;
		exit(1);
	}

	serv_address = argv[1];
	port_num = argv[2];

    char *dev;
    //char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* descr;
    struct bpf_program fp;      /* hold compiled program     */
    bpf_u_int32 maskp;          /* subnet mask               */
    bpf_u_int32 netp;           /* ip                        */
    u_char* args = NULL;


	static const int iterate = -1;
	char ebuf[PCAP_ERRBUF_SIZE];

	pcap_t *pd = NULL;
	cout << argv[1] << endl;
	pd = pcap_open_offline(argv[3], ebuf);

    if (pcap_loop(pd, iterate, pcap_callback, NULL) < 0) {
		(void)fprintf(stderr, "pcap_loop: error occurred\n");
	exit(1);
    }

    pcap_close(pd);




//
//    /* Options must be passed in as a string because I am lazy */
//    if(argc < 2){
//        fprintf(stdout,"Usage: %s numpackets \"options\"\n",argv[0]);
//        return 0;
//    }
//
//    /* grab a device to peak into... */
//    dev = pcap_lookupdev(errbuf);
//    if(dev == NULL)
//    { printf("%s\n",errbuf); exit(1); }
//
//    /* ask pcap for the network address and mask of the device */
//    pcap_lookupnet(dev,&netp,&maskp,errbuf);
//
//    /* open device for reading. NOTE: defaulting to
//     * promiscuous mode*/
//    descr = pcap_open_live(dev,BUFSIZ,1,-1,errbuf);
//    if(descr == NULL)
//    { printf("pcap_open_live(): %s\n",errbuf); exit(1); }
//
//
//    if(argc > 2)
//    {
//        /* Lets try and compile the program.. non-optimized */
//        if(pcap_compile(descr,&fp,argv[2],0,netp) == -1)
//        { fprintf(stderr,"Error calling pcap_compile\n"); exit(1); }
//
//        /* set the compiled program as the filter */
//        if(pcap_setfilter(descr,&fp) == -1)
//        { fprintf(stderr,"Error setting filter\n"); exit(1); }
//    }
//
//    /* ... and loop */
//    pcap_loop(descr,atoi(argv[1]),my_callback,args);
//
//    fprintf(stdout,"\nfinished\n");
//    return 0;
}




