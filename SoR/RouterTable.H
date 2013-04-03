#pragma once

#include "RouterTable.H"
#include <ktremotedb.h>
//#include "kcpolydb.h"
//using namespace kyototycoon;


struct RouterTableInfo{
	char nexthop[15];
	int ip_len;
	unsigned short port;
};



class RouterTable{
    private:
        kyototycoon::RemoteDB *rtable;
        kyototycoon::RemoteDB *ctable;
		map<string, RouterTableInfo *> map_routing_table;
    public:
        RouterTable();
        kyototycoon::RemoteDB* GetRouterTable(){return rtable;}
        kyototycoon::RemoteDB* GetCountTable(){return ctable;}
		string GetNextHop(string ip);
		unsigned short GetPort(string ip);
		string GetMapNextHop(string ip);
		unsigned short GetMapPort(string ip);
		int GetUpdateFlg();
		int SetUpdateFlg(int flg);
		int  InsertRoutingTable(kyototycoon::RemoteDB *rtable, string src_ip, string nexthop_ip, unsigned short nexthop_port);
		int  InsertMapRoutingTable(string src_ip, string nexthop_ip, unsigned short nexthop_port);
		int UpdateRoutingTable();
        ~RouterTable();
};
