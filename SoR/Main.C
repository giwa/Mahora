/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 *$Id: srim.C,v 5.27 2012-08-22 08:05:53 sin Exp $
 */
#include "Main.H"
#include "PacketCapture.H"
#include "PacketFileReader.H"
#include "Test.H"
#include "TestRule.H"
#include "Observer.H"
#include "SSS.H"
#include "../socket/PracticalSocket.H"      // For UDPSocket and SocketException
#include "Socket.H"      // For UDPSocket and SocketException

#include <functional>
#include <utility>


int main(int argc, char *argv[]){
//	signal(SIGINT, signal_handler);
//
//Initialize!!

if(argc != 2){
	cerr << "Usage: " << argv[0] << " config_file" << endl;
	return 1;
}

	//Read arguments
 	string config_file = argv[1];

	//Create instances
	config = new Config;
	config->init(config_file);
	config->show();

	master = new Master;
	rule_pool = new RulePool;
	stream_rebuild = new StreamRebuild;
	ip_filter = new IpFilter;
	http_decoder = new HttpDecoder;
	match_pre_filter = new MatchPreFilter;
	extractor = new Extractor;
	stream_pool = new StreamPool;
	tcp_conn_pool = new TcpConnPool;
	packet_clean = new PacketClean;
	observer = new Observer;
	gzip = new Gzip;
	file_writer = new FileWriter;

#ifdef USE_POSTGRES
	pgsql = new Pgsql;
	pgsql_saver = new PgsqlSaver;
	rule_loader_pgsql = new RuleLoaderPgsql;

	pgsql->Connect(config->get("dbname"),config->get("dbuser"),config->get("dbhost"),config->get("dbpass"));
	rule_loader_pgsql->Proc();
#else
	rule_loader_file = new RuleLoaderFile;
	rule_loader_file->Proc();
#endif

	rule_pool->ShowRules();

	router_table = new RouterTable;
	cout << "finish instance" << endl;
	//Create threads
	//SSS thread
/*
	pthread_t thread_sss_t;
	pthread_create(&thread_sss_t, NULL, thread_sss, NULL);
	pthread_detach(thread_sss_t);
	sleep(1);
*/

//	packetcap();



//	master->Proc(pkt);

//	sim_time = atol(config->get("sim_time").c_str());



	pthread_t sender_ID;
	pthread_t reciever_ID;

	/*create thread to send and recieve RT update*/

	pthread_create(&reciever_ID,NULL,&routeFunction_Recv,NULL);
	pthread_create(&sender_ID,NULL,&routeFunction_Send,NULL);


	pthread_join(sender_ID,NULL);
	pthread_join(reciever_ID,NULL);


int status = 0;
/*
	//Capture thread
	pthread_t thread_cap;
	status=pthread_create(&thread_cap, NULL, thread_packetcap,(void*)NULL);
	pthread_join( thread_cap, NULL );
//	while(1);
*/
    return 0;
}



