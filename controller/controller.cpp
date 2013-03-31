#include <ktremotedb.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace kyototycoon;

struct RouterTableInfo{
	char nexthop[15];
	int ip_len;
	int port;
};

// main routine
int main(int argc, char** argv) {

if(argc < 3){
	cout << "sourceIP ForwardingIP s" << endl;
	return 0;
}

  // create the database object
  RemoteDB db;

  // open the database
  if (!db.open()) {
    cerr << "open error: " << db.error().name() << endl;
  }


	RouterTableInfo *rtinfo = new RouterTableInfo;
	string dst_ip,src_ip;
	int port;
	char *buf;

	src_ip = argv[1];
	dst_ip = argv[2];
	port = 52000;

	memcpy(rtinfo->nexthop, dst_ip.c_str(), dst_ip.size());
	rtinfo->ip_len = dst_ip.size();
	rtinfo->port = port;
	cout << rtinfo->ip_len << endl;
	buf = (char *)rtinfo;
	db.set(src_ip.c_str(), src_ip.size(), buf, sizeof(RouterTableInfo) );


	src_ip = argv[2];
	dst_ip = argv[1];
	port = 52000;
	memcpy(rtinfo->nexthop, dst_ip.c_str(), dst_ip.size());
	rtinfo->ip_len = dst_ip.size();
	rtinfo->port = port;
	buf = (char *)rtinfo;
	db.set(src_ip.c_str(), src_ip.size(), buf, sizeof(RouterTableInfo) );

  // traverse records
  /*
  RemoteDB::Cursor* cur = db.cursor();
  cur->jump();
  string ckey, cvalue;
  while (cur->get(&ckey, &cvalue, NULL, true)) {
    cout << ckey << ":" << cvalue << endl;
  }
  delete cur;
*/
  // close the database
  if (!db.close()) {
    cerr << "close error: " << db.error().name() << endl;
  }

  return 0;
}
