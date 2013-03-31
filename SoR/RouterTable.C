#include "Include.H"
#include "Global.H"
#include "Stream.H"
#include "RouterTable.H"
//#include "kchashdb.h"
//using namespace kyotocabinet;

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
	RouterTableInfo *rtinfo;
	string dst_ip,src_ip;
	int port;
	char *buf;

	src_ip = "192.168.1.6";
	dst_ip = "192.168.1.2";
	port = 52000;
	memcpy(rtinfo->nexthop, dst_ip.c_str(), dst_ip.size());
	rtinfo->ip_len = dst_ip.size();
	rtinfo->port = port;
	buf = (char *)rtinfo;
	rtable->set(src_ip.c_str(), src_ip.size(), buf, sizeof(RouterTableInfo) );

	src_ip = "192.168.1.2";
	dst_ip = "192.168.1.6";
	port = 52000;
	memcpy(rtinfo->nexthop, dst_ip.c_str(), dst_ip.size());
	rtinfo->ip_len = dst_ip.size();
	rtinfo->port = port;
	buf = (char *)rtinfo;
	rtable->set(src_ip.c_str(), src_ip.size(), buf, sizeof(RouterTableInfo) );

}

RouterTable::~RouterTable(){
    rtable->close();
	ctable->close();
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
