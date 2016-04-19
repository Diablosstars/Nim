#include "Nim.h"
#include <WinSock2.h>
#include <iostream>
#include <string>


int playNim(SOCKET s, std::string serverName, std::string host, std::string port, std::string rockPileConfig, int player)
{
	std::cout << "You reached the game portion" << std::endl;
	std::cout << rockPileConfig << std::endl;
	char choice_str[80], newline;
	int choice = 0;
	const int FORFEIT = 3;
	bool myMove;
	bool gameOver = false;
	if (player == -1)
	{
		myMove = true;
	}
	else
	{
		myMove = false;
	}

	while (!gameOver)
	{
		if (myMove)
		{
			do {
				std::cout << std::endl << std::endl << std::endl;
				std::cout << "Choose an option:" << std::endl;
				std::cout << "   1 - Make a move" << std::endl;
				std::cout << "   2 - Send a chat" << std::endl;
				std::cout << "   3 - Forfeit game" << std::endl;
				std::cout << "Enter 1, 2 or 3: ";
				std::cin >> choice_str; std::cin.get(newline);
				choice = atoi(choice_str);

				switch (choice)
				{
					case 1:	sendChat(); break;
					case 2:	sendMove(); break;
					case 3: break;
					default: std::cout << std::endl << "Please enter a digit between 1 and 3." << std::endl;
				}
			} while (choice != FORFEIT);
		}
		else
		{
			std::cout << "Waiting for your opponent's move..." << std::endl << std::endl;
			int status = wait(s, 2, 0); //check for right time
			if (status > 0)
			{
				//parse recv str
			}
		}
	
	}

	return 0;
}

void sendChat()
{

}

void sendMove()
{

}