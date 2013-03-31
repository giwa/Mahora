#include "Include.H"
#include "Global.H"
#include "Stream.H"
#include "RouterTable.H"
//#include "kchashdb.h"
//using namespace kyotocabinet;


int InsertRoutingTable(kyototycoon::RemoteDB *rtable, string src_ip, string nexthop_ip, int nexthop_port){
	RouterTableInfo *rtinfo = new RouterTableInfo;
	char *buf;
	//cout << "------------------------------" << endl;
	memcpy(rtinfo->nexthop, nexthop_ip.c_str(), nexthop_ip.size());
	rtinfo->nexthop[nexthop_ip.size()] = '\0';
	rtinfo->ip_len = nexthop_ip.size();
	rtinfo->port = nexthop_port;
	buf = (char *)rtinfo;
	rtable->set(src_ip.c_str(), src_ip.size(), buf, sizeof(RouterTableInfo));
	delete rtinfo;

	return 0;
}



RouterTable::RouterTable(){
    rtable = new kyototycoon::RemoteDB;

	if(!rtable->open()){
		cerr << "open error: " << rtable->error().name() << endl;
	}

    ctable = new kyototycoon::RemoteDB;
	if(!ctable->open()){
		cerr << "open error: " << ctable->error().name() << endl;
	}


// initialize
	string nexthop_ip,src_ip;
	int nexthop_port;

	src_ip = "192.168.1.6";
	nexthop_ip = "192.168.1.2";
	nexthop_port = 52000;
	InsertRoutingTable(rtable, src_ip, nexthop_ip, nexthop_port);

	src_ip = "192.168.1.2";
	nexthop_ip = "192.168.1.6";
	nexthop_port = 52000;
	InsertRoutingTable(rtable, src_ip, nexthop_ip, nexthop_port);

}

RouterTable::~RouterTable(){
    rtable->close();
	ctable->close();

	delete rtable;
	delete ctable;
}

string RouterTable::GetNextHop(string ip){
	RouterTableInfo *rtinfo;
	char *buf;
	size_t num;
	buf = rtable->get(ip.c_str(), ip.size(), &num);
	if(buf == NULL){
		return "0";
	}
	rtinfo = (RouterTableInfo *)buf;
	rtinfo->nexthop[rtinfo->ip_len ]='\0';
	return rtinfo->nexthop;
}

unsigned short RouterTable::GetPort(string ip){
	RouterTableInfo *rtinfo;
	char *buf;
	size_t num;
	buf = rtable->get(ip.c_str(), ip.size(), &num);
	if(buf == NULL){
		return 0;
	}
	rtinfo = (RouterTableInfo *)buf;
	return rtinfo->port;
}
