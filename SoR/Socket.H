#include "Include.H"
#include "Global.H"
#include "../socket/PracticalSocket.H"      // For UDPSocket and SocketException


#define routerlistnPort 52000

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;


//router queue definition

void* routeFunction_Recv(void * param);
void* routeFunction_Send(void * param);







