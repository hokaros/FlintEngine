#include <iostream>
#include "Client.h"

Client* client;
std::thread* clientThread;

int main()
{
	std::cout << "Hello World!\n";
	client = new Client("127.0.0.1", 80);
	client->Connect();

	client->GameProtocol();

	std::string command = "";

	while (command != "quit")
	{
		std::cout << "# ";
		std::cin >> command;

		if (command == "quit") break;
		else if (command == "") continue;
		else std::cout << "Nieprawidlowa komenda\n";
	}

	return 0;
}