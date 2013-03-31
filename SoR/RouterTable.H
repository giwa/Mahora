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
    public:
        RouterTable();
        kyototycoon::RemoteDB* GetRouterTable(){return rtable;}
        kyototycoon::RemoteDB* GetCountTable(){return ctable;}
		string GetNextHop(string ip);
		unsigned short GetPort(string ip);
        ~RouterTable();
};
