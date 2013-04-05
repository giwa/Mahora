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
	unsigned char* packet2;			// ponter to packet binary
	unsigned char* l3_header;
	unsigned char* l3_header2;
	struct ip* ip_header;
	struct ip* ip_header2;
	struct in_addr src_ip;
	struct in_addr src_ip2;
	struct in_addr dst_ip;
	struct in_addr dst_ip2;
	struct timeval timestamp;

	cnt++;
	PacketCnt *pcnt;
	int packet_size = sizeof(struct pcap_pkthdr) + h->caplen;
	cout << h->caplen << endl;
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


	struct tcphdr* tcp_header;		// pointer to TCP header structure



	unsigned short echoServPort = Socket::resolveService(port_num, "udp");


	try {
	memcpy(data->src_ip,"192.168.1.6",15);
	data->src_port = sock->getLocalPort();
	data->sor_flg = 0;

	  sockaddr_in addr;
	unsigned int addr_len = sizeof(addr);

	cout << "src_ip sim: "<<  data->src_ip << endl;
	// Send the string to the server
	sock->sendTo(data, sizeof(SoRData), serv_address, echoServPort);
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




