#pragma once
#include "Framework.h"
#include <Windows.h>

class Receive {

public:
	Receive(sf::TcpSocket &_socket, std::size_t &_received, std::vector<std::string> &_aMensajes) {
		socket = &_socket;
		received = &_received;
		aMensajes = &_aMensajes;
	};
	sf::TcpSocket *socket;
	std::size_t *received;
	std::vector<std::string> *aMensajes;

	void ReceiveFunction();
};

void Receive::ReceiveFunction() {

	char buffer[2000];

	while (true) {
		socket->receive(buffer, sizeof(buffer), *received);

		if (received > 0)
		{
			if (strcmp(buffer, "Disconnected") == 0)
			{
				socket->receive(buffer, sizeof(buffer), *received);
				aMensajes->push_back(buffer);
				Sleep(5000);
				socket->disconnect();
				exit(0);
			}
			aMensajes->push_back(buffer);
			if (aMensajes->size() > 25)
			{
				aMensajes->erase(aMensajes->begin(), aMensajes->begin() + 1);
			}

		}
	}
	
}