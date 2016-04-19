//Nim.h
#include <WinSock2.h>
#include <string>

static char Nim_UDPPORT[] = "29333";			

#define Nim_QUERY				"Who?"
#define Nim_NAME				"Name="
#define Nim_CHALLENGE			"Player="
#define Nim_CHALLENGE_ACCEPTED	"YES"
#define Nim_CHALLENGE_REJECTED	"NO"
#define Nim_CHALLENGE_RESPONSE	"Great!"

const int v4AddressSize = 16;
const int MAX_INTERFACES = 20;
const int MAX_HOST_SIZE = 16;
const int MAX_SERVICE_SIZE = 10;
const int WAIT_TIME = 60;
const int MAX_SEND_BUF = 2048;
const int MAX_RECV_BUF = 2048;
const int MAX_SERVERS = 100;


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
int playNim(SOCKET, std::string, std::string, std::string, std::string, int);//edit for sending rock pile config
int server_main(int, char*[], std::string);
int client_main(int, char*[], std::string);
void sendChat();
void sendMove();


//std::random_device rd;     // only used once to initialise (seed) engine
//std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
//std::uniform_int_distribution<int> piles(3, 9); // guaranteed unbiased
//std::uniform_int_distribution<int> rocks(1, 20); // guaranteed unbiased
//
//int numberPiles = piles(rng);
//int numberRocks = rocks(rng);
//
//std::string rockPileConfig;
//
//rockPileConfig[0] = numberPiles;
//
//for (int i = 0; i < numberPiles; i++)
//{
//	if (numberRocks < 10)
//	{
//		rockPileConfig[i] = '0';
//		rockPileConfig[i + 1] = numberRocks;
//		i++;
//	}
//	else
//	{
//		rockPileConfig[i] = numberRocks;
//	}
//}
