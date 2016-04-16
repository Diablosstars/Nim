#include "Nim.h"
#include <WinSock2.h>
#include <iostream>

// getServers.cpp
//  Given a socket, a broadcast address and a port number, this function returns the number
//  of servers as well as an array of structs.  Each struct contains the name, IP_Address and 
//  port number of a remote server.

//	IN parameters:
//	SOCKET s				= Allocate UDP socket handle that can be used for communcations
//	char *broadcastAddress	= The broadcast address that should be used for current LAN
//	char *broadcastPort		= The port that should be used for the broadcast

//	OUT parameters:
//	ServerStruct server[]	= An array that contains the structs.  The members of each struct
//							  contain the name, IP Address and port number of remote server.
//	numServers				= Number of elements in the server[] array

//	Function return value	= Number of remote servers (size of server[] array)

int getServers(SOCKET s, char *broadcastAddress, char *broadcastPort, ServerStruct server[])
{
	char host[MAX_HOST_SIZE];
	char port[MAX_SERVICE_SIZE];
	char buf[MAX_SEND_BUF];

	int numServers = 0;

	// Send TicTacToe_QUERY to broadcastAddress using broadcastPort
	UDP_send(s, Nim_QUERY, sizeof(Nim_QUERY) + 1, broadcastAddress, broadcastPort);

	// Receive incoming UDP datagrams (with a maximum of 2 second wait before each recv() function call)
	// As you read datagrams, if they start with the prefix: TicTacToe_NAME, parse out the server's name
	// and add the name, host address and port number to the server[] array.  Don't forget to increment numServers.
	int status = wait(s, 2, 0);
	if (status > 0) 
	{
		int len = 0;

		len = UDP_recv(s, buf, MAX_RECV_BUF, broadcastAddress, broadcastPort);

		while (status > 0 && len > 0) {
			std::string nameString = Nim_NAME;
			std::string tempBuffString = buf;

			//This grabs the first five characters of the message recieved and 
			//compares it to the expected player string. In this case, we are
			//expecting "Name=" as the first 5 characters. 
			tempBuffString = tempBuffString.substr(0, 5);

			if (nameString == tempBuffString)
			{
				std::string buffString = buf;

				buffString = buffString.substr(5, buffString.length());
				server[numServers].name = buffString;
				server[numServers].host = broadcastAddress;
				server[numServers].port = broadcastPort;
				numServers++;
			}

			// Now we'll see if there is another response.
			status = wait(s, 2, 0);

			if (status > 0)
			{
				len = UDP_recv(s, buf, MAX_RECV_BUF, host, port);
			}
		}
	}
	return numServers;
}
