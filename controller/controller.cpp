#include <ktremotedb.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>


using namespace std;
using namespace kyototycoon;

struct RouterTableInfo{
	char nexthop[15];
	int ip_len;
	int port;
};

int UpdateRoutingTable(RemoteDB *db, string src_ip, string nexthop_ip, int nexthop_port){
	RouterTableInfo *rtinfo = new RouterTableInfo;
	char *buf;

	memcpy(rtinfo->nexthop, nexthop_ip.c_str(), nexthop_ip.size());
	rtinfo->ip_len = nexthop_ip.size();
	rtinfo->port = nexthop_port;
	buf = (char *)rtinfo;
	db->set(src_ip.c_str(), src_ip.size(), buf, sizeof(RouterTableInfo));
	delete rtinfo;
}

int ShowRoutingTable(RemoteDB *db){
	RemoteDB::Cursor* cur = db->cursor();

	cur->jump();
	string ckey, cvalue;
	cout << "src_ip" << "	" << "nexthop_ip" << "	" << "nexthop_port" << endl;
	RouterTableInfo *rtinfo;
	while (cur->get(&ckey, &cvalue, NULL, true)) {
		rtinfo = (RouterTableInfo *)cvalue.c_str();
		cout << ckey << "	" << rtinfo->nexthop << "	" << rtinfo->port  <<endl;
	}
	delete cur;

}



// main routine
int main(int argc, char** argv) {
/*
	if(argc < 3){
	cout << "sourceIP ForwardingIP" << endl;
	return 0;
	}
*/
    char *ptr;
    string line;
    while(ptr = readline())
    {
		
        line = ptr;
        cout << line << endl;
        free(ptr);
    }


	// create the database object
	RemoteDB db;

	// open the database
	if (!db.open()) {
	cerr << "open error: " << db.error().name() << endl;
	}

	string nexthop_ip,src_ip;
	int nexthop_port;

	src_ip = argv[1];
	nexthop_ip = argv[2];
	nexthop_port = 52000;
	UpdateRoutingTable(&db, src_ip, nexthop_ip, nexthop_port);

	src_ip = argv[2];
	nexthop_ip = argv[1];
	nexthop_port = 52000;
	UpdateRoutingTable(&db, src_ip, nexthop_ip, nexthop_port);

	ShowRoutingTable(&db);

	// close the database
	if (!db.close()) {
		cerr << "close error: " << db.error().name() << endl;
	}

	return 0;
}
