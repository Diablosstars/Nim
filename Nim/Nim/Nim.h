//Nim.h
#include <WinSock2.h>
#include <string>

static char Nim_UDPPORT[] = "29333";			

#define Nim_QUERY		"Who?"
#define Nim_NAME		"Name="
#define Nim_CHALLENGE	"Player="

const int v4AddressSize = 16;
const int MAX_INTERFACES = 20;
const int MAX_HOST_SIZE = 16;
const int MAX_SERVICE_SIZE = 10;
const int WAIT_TIME = 60;
const int MAX_SEND_BUF = 2048;
const int MAX_RECV_BUF = 2048;
const int MAX_SERVERS = 100;
//const int X_PLAYER = 1;
//const int O_PLAYER = 2;
//const int ABORT = -1;
//const int noWinner = 0;
//const int xWinner = 1;
//const int oWinner = 2;
//const int TIE = 3;
//
//static char board[10];

struct interfaceInfo {
	char IPAddress[v4AddressSize];
	char netmask[v4AddressSize];
	char broadcastAddress[v4AddressSize];
	bool isUP;
	bool isLoopback;
	bool isPoint2Point;
	bool canBroadcast;
	bool canMulticast;
};

struct ServerStruct {
	std::string name;
	std::string host;
	std::string port;
};

SOCKET connectsock(char*, char*, char*);
SOCKET passivesock(char*, char*);
int UDP_recv(SOCKET, char*, int, char*, char*);
int UDP_send(SOCKET, char*, int, char*, char*);
int wait(SOCKET, int, int);
int loadInterfaceInfo(interfaceInfo[]);
char* timestamp();
int getServers(SOCKET, char*, char*, ServerStruct[]);
int playNim(SOCKET, std::string, std::string, std::string, int);
int server_main(int, char*[], std::string);
int client_main(int, char*[], std::string);
