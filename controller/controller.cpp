#include <ktremotedb.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <utility>

using namespace std;
using namespace kyototycoon;

struct RouterTableInfo{
	char nexthop[15];
	int ip_len;
	int port;
};

int UpdateRoutingTable(string serv_ip, string src_ip, string nexthop_ip, int nexthop_port){
	RouterTableInfo *rtinfo = new RouterTableInfo;
	char *buf;

	RemoteDB db;
	if (!db.open(serv_ip)) {
	cerr << "open error: " << db.error().name() << endl;
	}

	memcpy(rtinfo->nexthop, nexthop_ip.c_str(), nexthop_ip.size());
	rtinfo->nexthop[nexthop_ip.size()] = '\0';
	rtinfo->ip_len = nexthop_ip.size();
	rtinfo->port = nexthop_port;
	buf = (char *)rtinfo;
	db.set(src_ip.c_str(), src_ip.size(), buf, sizeof(RouterTableInfo));
	delete rtinfo;

	if (!db.close()) {
		cerr << "close error: " << db.error().name() << endl;
	}
}


int RemoveRoutingTable(string serv_ip, string src_ip){

	RemoteDB db;
	if (!db.open(serv_ip)) {
	cerr << "open error: " << db.error().name() << endl;
	}

	db.remove(src_ip);

	if (!db.close()) {
		cerr << "close error: " << db.error().name() << endl;
	}
}

int ClearRoutingTable(string serv_ip){

	RemoteDB db;
	if (!db.open(serv_ip)) {
	cerr << "open error: " << db.error().name() << endl;
	}

	db.clear();

	if (!db.close()) {
		cerr << "close error: " << db.error().name() << endl;
	}
}

int ShowRoutingTable(string serv_ip){
	RemoteDB db;
	if (!db.open(serv_ip)) {
	cerr << "open error: " << db.error().name() << endl;
	}

	RemoteDB::Cursor* cur = db.cursor();

	cur->jump();
	string ckey, cvalue;
	cout << "src_ip" << "	" << "nexthop_ip" << "	" << "nexthop_port" << endl;
	RouterTableInfo *rtinfo;
	while (cur->get(&ckey, &cvalue, NULL, true)) {
		rtinfo = (RouterTableInfo *)cvalue.c_str();
		cout << ckey << "	" << rtinfo->nexthop << "	" << rtinfo->port  <<endl;
	}
	delete cur;

	if (!db.close()) {
		cerr << "close error: " << db.error().name() << endl;
	}

}



// main routine
int main(int argc, char** argv) {
    char *ptr, *next;
    string line, op;
	int error = 0;
	map<int, string> update_ip;

    while(ptr = readline(NULL))
    {
        line = ptr;
		//op = strtok(ptr, " ");
		op = line.substr(0,line.find(" "));

		if(!op.empty()){
			cout << "op " << op << endl;

			if(op == "show"){
				string ip;
				ip = line.substr(line.find(" ") + 1);
				//ip error process
				if( inet_addr(ip.c_str()) != INADDR_NONE){
					//cout << "ip: " <<  ip << endl;
					ShowRoutingTable(ip);
				}else{
					cout << "input correct IP address" << endl;
				}
			}else if(op == "update"){
				error = 0;
				string ip;
				int nexthop_port;

				next = strtok(ptr, " ");
				for(int i = 0; i <= 2; i++){
					next = strtok(NULL, " ");
					if (next != NULL){
						if(inet_addr(next) != INADDR_NONE){
							cout << next << endl;
							update_ip[i] = next;
						}else{
							error++;
						}
					}
				}

				next = strtok(NULL, " ");
				if(next != NULL){
					nexthop_port = atoi(next);
					if(nexthop_port < 0){
						error++;
					}
				}
			//	cout << "error " << error << endl;
				if(error == 0){
					UpdateRoutingTable(update_ip[0], update_ip[1], update_ip[2], nexthop_port);
					UpdateRoutingTable(update_ip[0], update_ip[2], update_ip[1], nexthop_port);
				}
				/*
				cout << nexthop_port << endl;
				cout << "update ip1 " << update_ip[0] << endl;
				cout << "update ip2 " << update_ip[1] << endl;
				cout << "update ip3 " << update_ip[2] << endl;
				*/
				//ip = line.substr(line.find(" ") + 1, line.find(" ", ));
				
			}else if(op == "delete"){
				error = 0;
				next = strtok(ptr, " ");
				for(int i = 0; i <= 1; i++){
					next = strtok(NULL, " ");
					if (next != NULL){
						if(inet_addr(next) != INADDR_NONE){
							cout << next << endl;
							update_ip[i] = next;
						}else{
							error++;
						}
					}
				}
				if(error == 0){
					RemoveRoutingTable(update_ip[0], update_ip[1]);
				}
			}else if(op == "clear"){
				error = 0;
				next = strtok(ptr, " ");
				next = strtok(NULL, " ");
				if (next != NULL){
					if(inet_addr(next) != INADDR_NONE){
						cout << next << endl;
						update_ip[0] = next;
					}else{
						error++;
					}
				}
				if(error == 0){
					ClearRoutingTable(update_ip[0]);
				}
			}else{
				cout << "Please input correct operation" << endl;
				cout << "	update RoutingTableIP srcIP nexthopIP nexthopPort" << endl;
				cout << "	delete RoutingTableIP srcIP" << endl;
				cout << "	clear RoutingTableIP" << endl;
				cout << "	show RoutingTableIP" << endl;
			}
		}else{
			return 0;
		}
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
	//UpdateRoutingTable(&db, src_ip, nexthop_ip, nexthop_port);

	src_ip = argv[2];
	nexthop_ip = argv[1];
	nexthop_port = 52000;
	//UpdateRoutingTable(&db, src_ip, nexthop_ip, nexthop_port);


	// close the database
	if (!db.close()) {
		cerr << "close error: " << db.error().name() << endl;
	}

	return 0;
}
