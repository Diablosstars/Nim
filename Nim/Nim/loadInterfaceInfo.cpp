#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Nim.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
using namespace std;

int loadInterfaceInfo(interfaceInfo networkInterface[])
{
	int nNumInterfaces = 0;

	// Initialize array of structs: networkInterface[]
	for (int i = 0; i < MAX_INTERFACES; i++) {
		strcpy_s(networkInterface[i].IPAddress, v4AddressSize, "");
		strcpy_s(networkInterface[i].netmask, v4AddressSize, "");
		strcpy_s(networkInterface[i].broadcastAddress, v4AddressSize, "");
		networkInterface[i].isUP = false;
		networkInterface[i].isLoopback = false;
		networkInterface[i].isPoint2Point = false;
		networkInterface[i].canBroadcast = false;
		networkInterface[i].canMulticast = false;
	}

	SOCKET sd = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
	if (sd == SOCKET_ERROR) {
		cerr << "Failed to get a socket. Error " << WSAGetLastError() <<
			endl; return -1;
	}

	INTERFACE_INFO InterfaceList[MAX_INTERFACES];
	unsigned long nBytesReturned;
	if (WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, &InterfaceList,
		sizeof(InterfaceList), &nBytesReturned, 0, 0) == SOCKET_ERROR) {
		cerr << "Failed calling WSAIoctl: error " << WSAGetLastError() <<
			endl;
		return -1;
	}

	nNumInterfaces = nBytesReturned / sizeof(INTERFACE_INFO);
	for (int i = 0; i < nNumInterfaces; ++i) {
		sockaddr_in *pAddress;

		// Extract and store IP address and netmask (strings) of ith interface
		pAddress = (sockaddr_in *)& (InterfaceList[i].iiAddress);
		strcpy_s(networkInterface[i].IPAddress, v4AddressSize, inet_ntoa(pAddress->sin_addr));
		pAddress = (sockaddr_in *)& (InterfaceList[i].iiNetmask);
		strcpy_s(networkInterface[i].netmask, v4AddressSize, inet_ntoa(pAddress->sin_addr));

		// Calculate and store the broadcast address string of the ith interface
		unsigned long addr = InterfaceList[i].iiAddress.AddressIn.sin_addr.S_un.S_addr;
		unsigned long mask = InterfaceList[i].iiNetmask.AddressIn.sin_addr.S_un.S_addr;
		unsigned long bCast = addr | (~mask);
		struct sockaddr_in temp;
		temp.sin_addr.S_un.S_addr = bCast;
		strcpy_s(networkInterface[i].broadcastAddress, v4AddressSize, inet_ntoa(temp.sin_addr));

		// Extract and store the flag bits: isUp, isPoint2Point, isLoopback, canBroadcast and canMulticast
		u_long nFlags = InterfaceList[i].iiFlags;
		if (nFlags & IFF_UP) { networkInterface[i].isUP = true; }
		if (nFlags & IFF_POINTTOPOINT) { networkInterface[i].isPoint2Point = true; }
		if (nFlags & IFF_LOOPBACK) { networkInterface[i].isLoopback = true; }
		if (nFlags & IFF_BROADCAST) { networkInterface[i].canBroadcast = true; }
		if (nFlags & IFF_MULTICAST) { networkInterface[i].canMulticast = true; }
	}

	return nNumInterfaces;
}