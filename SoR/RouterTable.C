#include "Include.H"
#include "Global.H"
#include "Stream.H"
#include "RouterTable.H"
#include <stdio.h>      /* printf, fgets */
//#include "kchashdb.h"
//using namespace kyotocabinet;

string	prefix = "rt";


char *itoa( int value, char buff[], int radix )
{
	static const char table[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	char *head = buff;
	char *tail = buff;
	char temp;

	// マイナス符号を文字列に変換
	if ( value < 0 ){
		*tail++ = '-';
		value = -value;
	}
	// 整数値を文字列に変換
	if ( value == 0 ){
		*tail++ = '0';
	}
	else for ( head = tail ; value != 0 ; value /= radix ){
		*tail++ = table[ value % radix ];
	}
	*tail = '\0';
	// 文字列の左右入れ替え
	for ( tail-- ; head < tail ; head++, tail-- ){
		temp = *head;
		*head = *tail;
		*tail = temp;
	}
	return buff;
}




int RouterTable::InsertRoutingTable(kyototycoon::RemoteDB *rtable, string src_ip, string nexthop_ip, unsigned short nexthop_port){
	RouterTableInfo *rtinfo = new RouterTableInfo;
	char *buf;
	//cout << "------------------------------" << endl;
	src_ip = prefix + src_ip;
	memcpy(rtinfo->nexthop, nexthop_ip.c_str(), nexthop_ip.size());
	rtinfo->nexthop[nexthop_ip.size()] = '\0';
	rtinfo->ip_len = nexthop_ip.size();
	rtinfo->port = nexthop_port;
	buf = (char *)rtinfo;
	rtable->set(src_ip.c_str(), src_ip.size(), buf, sizeof(RouterTableInfo));
	delete rtinfo;

	return 0;
}


int RouterTable::InsertMapRoutingTable(string src_ip, string nexthop_ip, unsigned short nexthop_port){
	RouterTableInfo *rtinfo = new RouterTableInfo;

	src_ip = prefix + src_ip;
	memcpy(rtinfo->nexthop, nexthop_ip.c_str(), nexthop_ip.size());
	rtinfo->nexthop[nexthop_ip.size()] = '\0';
	rtinfo->ip_len = nexthop_ip.size();
	rtinfo->port = nexthop_port;
	map_routing_table[src_ip] = rtinfo;

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
	unsigned short nexthop_port;

	SetUpdateFlg(0);

	src_ip = "192.168.1.6";
	nexthop_ip = "192.168.1.2";
	nexthop_port = 52000;
	InsertRoutingTable(rtable, src_ip, nexthop_ip, nexthop_port);
	InsertMapRoutingTable(src_ip, nexthop_ip, nexthop_port);

	src_ip = "192.168.1.2";
	nexthop_ip = "192.168.1.6";
	nexthop_port = 52000;
	InsertRoutingTable(rtable, src_ip, nexthop_ip, nexthop_port);
	InsertMapRoutingTable(src_ip, nexthop_ip, nexthop_port);

	//cout << "src_ip: " << GetMapPort(src_ip) << endl;
	//cout << "0: " << GetMapPort("fdsfa") << endl;

	//cout << "src_ip: " << src_ip << ":" << map_routing_table[src_ip]->nexthop << endl;

}

RouterTable::~RouterTable(){
    rtable->close();
	ctable->close();

	delete rtable;
	delete ctable;
}

string RouterTable::GetNextHop(string ip){
	RouterTableInfo *rtinfo = new RouterTableInfo;
	char *buf;
	size_t num;

	ip = prefix + ip;
	buf = rtable->get(ip.c_str(), ip.size(), &num);
	if(buf == NULL){
		return "0";
	}
	rtinfo = (RouterTableInfo *)buf;
	rtinfo->nexthop[rtinfo->ip_len]='\0';

	return rtinfo->nexthop;
}

string RouterTable::GetMapNextHop(string ip){
	RouterTableInfo *rtinfo = new RouterTableInfo;
	char *buf;
	size_t num;

	ip = prefix + ip;
	rtinfo = map_routing_table[ip];
	if(rtinfo == NULL){
		return "0";
	}
	rtinfo->nexthop[rtinfo->ip_len]='\0';

	return rtinfo->nexthop;
}

unsigned short RouterTable::GetPort(string ip){
	RouterTableInfo *rtinfo;
	char *buf;
	size_t num;

	ip = prefix + ip;
	buf = rtable->get(ip.c_str(), ip.size(), &num);
	if(buf == NULL){
		return 0;
	}
	rtinfo = (RouterTableInfo *)buf;

	return rtinfo->port;
}

unsigned short RouterTable::GetMapPort(string ip){
	RouterTableInfo *rtinfo;
	size_t num;

	ip = prefix + ip;
	rtinfo = map_routing_table[ip];
	if(rtinfo == NULL){
		return 0;
	}

	return rtinfo->port;
}


int RouterTable::GetUpdateFlg(){
	string val;
	int rtn;

	rtable->get("updateflg", &val);
	rtn =  atoi(val.c_str());
	return rtn;
}


int RouterTable::SetUpdateFlg(int flg){
	string val;
	int rtn;
	char buf[10];

	val = itoa(flg,buf, 10);
	rtn = rtable->set("updateflg",val);

	return rtn;
}

int RouterTable::UpdateRoutingTable(){
	string key, val, chk;
	RouterTableInfo *rtinfo;
	kyototycoon::RemoteDB::Cursor* cur = rtable->cursor();
	map_routing_table.clear();

	cur->jump();
	while(cur->get(&key, &val, NULL, true)){
		rtinfo = (RouterTableInfo *)val.c_str();
		map_routing_table[key] = rtinfo;
	}
	SetUpdateFlg(0);

	return 0;
}


