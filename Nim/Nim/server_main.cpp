// server_main.cpp
//   This function serves as the "main" function for the server-side
#include "Nim.h"
#include <iostream>
#include <string>
#include <WinSock2.h>

int server_main(int argc, char *argv[], std::string playerName)
{
	SOCKET s;
	char buf[MAX_RECV_BUF];
	std::string host;
	std::string port;
	char response_str[MAX_SEND_BUF];

	s = passivesock(Nim_UDPPORT, "udp");

	std::cout << std::endl << "Waiting for a challenge..." << std::endl;
	int len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
	std::cout << timestamp() << " - Received: " << buf << std::endl;

	bool finished = false;
	while (!finished) {
		if (strcmp(buf, Nim_QUERY) == 0) {
			// Respond to name query
			strcpy_s(response_str, Nim_NAME);
			strcat_s(response_str, playerName.c_str());
			UDP_send(s, response_str, strlen(response_str) + 1, (char*)host.c_str(), (char*)port.c_str());
			std::cout << timestamp() << " - Sending: " << response_str << std::endl;

		}
		else if (strncmp(buf, Nim_CHALLENGE, strlen(Nim_CHALLENGE)) == 0) {
			// Received a challenge  
			char *startOfName = strstr(buf, Nim_CHALLENGE);
			if (startOfName != NULL) {
				std::cout << std::endl << "You have been challenged by " << startOfName + strlen(Nim_CHALLENGE) << std::endl;
			}

			char buf[MAX_SEND_BUF];
			std::string answer_str;
			std::cout << "Do you accept this challenge? ";
			std::getline(std::cin, answer_str);

			if (answer_str[0] == 'y' || answer_str[0] == 'Y')
			{
				strcpy_s(buf, Nim_CHALLENGE_ACCEPTED);
				UDP_send(s, buf, strlen(buf) + 1, (char*)host.c_str(), (char*)port.c_str());
				// Play the game. 
				//int winner = playNim(s, (char*)playerName.c_str(), (char*)host.c_str(), (char*)port.c_str(), O_PLAYER);
				playNim(s, (char*)playerName.c_str(), (char*)host.c_str(), (char*)port.c_str(), 1);
				finished = true;
			}
			else
			{
				strcpy_s(buf, Nim_CHALLENGE_REJECTED);
				UDP_send(s, buf, strlen(buf) + 1, (char*)host.c_str(), (char*)port.c_str());
			}
		}

		if (!finished) {
			char previous_buf[MAX_RECV_BUF];		strcpy_s(previous_buf, buf);
			std::string previous_host;				previous_host = host;
			std::string previous_port;				previous_port = port;

			// Check for duplicate datagrams (can happen if broadcast enters from multiple ethernet connections)
			bool newDatagram = false;
			int status = wait(s, 1, 0);	// We'll wait a second to see if we receive another datagram
			while (!newDatagram && status > 0) {
				len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
				std::cout << timestamp() << " - Received: " << buf << std::endl;
				if (strcmp(buf, previous_buf) == 0 &&		// If this datagram is identical to previous one, ignore it.
					host == previous_host &&
					port == previous_port) {
					status = wait(s, 1, 0);			// Wait another second (still more copies?)
				}
				else {
					newDatagram = true;		// if not identical to previous one, keep it!
				}
			}

			// If we waited one (or more seconds) and received no new datagrams, wait for one now.
			if (!newDatagram) {
				len = UDP_recv(s, buf, MAX_RECV_BUF, (char*)host.c_str(), (char*)port.c_str());
				std::cout << timestamp() << " - Received: " << buf << std::endl;
			}
		}
	}
	closesocket(s);

	return 0;
}